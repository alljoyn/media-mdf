/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include "ContentServerBusObject.h"
#include <ContentConstants.h>
#include <LogModule.h>

using namespace qcc;

namespace mdf {

ContentServerBusObject::ContentServerBusObject(const String& busObjectPath, MdfContentSource* contentSource) :  BusObject(busObjectPath.c_str()), m_ContentSource(contentSource)
{
    QCC_DbgTrace(("ContentServerBusObject: created"));
}

ContentServerBusObject::~ContentServerBusObject()
{
}

QStatus ContentServerBusObject::CreateInterface(BusAttachment& busAttachment)
{
    QCC_DbgTrace(("ContentServerBusObject::CreateInterface"));

    QStatus status  = busAttachment.CreateInterfacesFromXml(s_CONTENT_SOURCE_IF_XML.c_str());
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::Failed to create interfaces from XML!"));
        return status;
    }

    const InterfaceDescription* contentSourceInterface = busAttachment.GetInterface(s_CONTENT_INTERFACE.c_str());

    status = CheckInterface(contentSourceInterface);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::CheckInterface failed "));
        return status;
    }

    status = AddContentMethodHandlers(busAttachment);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::AddContentMethodHandlers failed "));
        return status;
    }

    QCC_DbgTrace(("m_BusAttachment->RegisterBusObject"));
    status = busAttachment.RegisterBusObject(*this);
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to register the bus object"));
        return status;
    }

    return ER_OK;
}

QStatus ContentServerBusObject::CheckInterface(const InterfaceDescription* contentSourceInterface)
{
    QCC_DbgTrace(("CheckInterface"));

    if (contentSourceInterface == NULL) {
        QCC_LogError(ER_INVALID_ADDRESS, ("Failed to create interfaces from XML!"));
        return ER_INVALID_ADDRESS;
    }

    if (contentSourceInterface->GetMember(s_METHOD_BROWSE.c_str()) == NULL) {
        QCC_LogError(ER_INVALID_ADDRESS, ("Retrieving Browse method xml returned NULL"));
        return ER_BUS_UNKNOWN_INTERFACE;
    }

    if (contentSourceInterface->GetMember(s_METHOD_GETITEM.c_str()) == NULL) {
        QCC_LogError(ER_BUS_UNKNOWN_INTERFACE, ("Retrieving GetItem method xml returned NULL"));
        return ER_BUS_UNKNOWN_INTERFACE;
    }

    return ER_OK;
}

QStatus ContentServerBusObject::AddContentMethodHandlers(BusAttachment& busAttachment)
{
    QCC_DbgTrace(("AddContentMethodHandlers"));

    /** Add the test interface to this object */
    const InterfaceDescription* contentSourceInterface = busAttachment.GetInterface(s_CONTENT_INTERFACE.c_str());
    assert(contentSourceInterface);

    AddInterface(*contentSourceInterface);

    const MethodEntry methodEntries[] =
    {
        { contentSourceInterface->GetMember(s_METHOD_BROWSE.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::Browse), },
        { contentSourceInterface->GetMember(s_METHOD_GETITEM.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::GetItem), },
        { contentSourceInterface->GetMember(s_METHOD_GETSORTABLEFIELDS.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::GetSortableFields), },
        { contentSourceInterface->GetMember(s_METHOD_GETCONTENTSOURCEINFO.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::GetContentSourceInfo), },
        { contentSourceInterface->GetMember(s_METHOD_MODIFYOPTIONALMETADATA.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::ModifyOptionalMetaData) },
        { contentSourceInterface->GetMember(s_METHOD_SEARCH.c_str()),
          static_cast<MessageReceiver::MethodHandler>(&ContentServerBusObject::Search) }
    };

    QStatus status = AddMethodHandlers(methodEntries, sizeof(methodEntries) / sizeof(methodEntries[0]));
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to register method handlers"));
    }
    return status;
}

void ContentServerBusObject::ObjectRegistered()
{
    QCC_DbgTrace(("AddContentMethodHandlers"));

    BusObject::ObjectRegistered();
}

/*
   This is what the return value should look like

   (utuua(sssa{sv}))
   u – The number of results returned.
   t - The maximum number of results that can be returned.
   u – The maximum number of results to return e.g. 20 used for paging.
   t – The offset within the result.
   a(sss):
   s - itemID
   s - title
   s - type
   a{sv} : Arbitrary properties
   s - key
   v - value
 */

void ContentServerBusObject::ReplySearchMsgArgs(Message& msg, NodeList* nodes, uint64_t offset, uint32_t limit)
{
    QCC_DbgTrace(("ContentServerBusObject::ReplySearchMsgArgs"));
    //  ututa(sssa{ss})

    MsgArgList nodeList(nodes->size());

    for (size_t i = 0; i < nodeList.size(); i++) {
        Node* node = nodes->at(i);

        //Set properties
        PropertiesMap& propertiesMap = node->GetProperties();
        MsgArgList propertiesList(propertiesMap.size());
        int index = 0;

        for (PropertiesMap::iterator it = propertiesMap.begin(); it != propertiesMap.end(); it++) {
            QCC_DbgTrace(("ContentServerBusObject::ReplySearchMsgArgs Set property key=%s, value=%s", it->first.c_str(), it->second.c_str()));
            QStatus status = propertiesList[index].Set("{ss}", it->first.c_str(), it->second.c_str());
            if (status != ER_OK) {
                QCC_LogError(status, ("ContentServerBusObject::ReplySearchMsgArgs: propertiesList[i].Set failed"));
                continue;
            }
        }

        QCC_DbgTrace(("ContentServerBusObject::ReplySearchMsgArgs Set node itemId=%s, title=%s", node->GetItemID().c_str(), node->GetTitle().c_str()));
        QStatus status = nodeList[i].Set("(sssa{ss})", node->GetItemID().c_str(), node->GetTitle().c_str(), node->GetTypeAsString().c_str(), propertiesList.size(), &propertiesList.front());
        if (status != ER_OK) {
            QCC_LogError(status, ("ContentServerBusObject::ReplySearchMsgArgs: nodeList[i].Set failed"));
            continue;
        }
    }

    MsgArg replyMsg;
    QCC_DbgTrace(("ContentServerBusObject::ReplySearchMsgArgs Set msgReply size=%d, limit=%d", nodeList.size(), limit));
    QStatus status = replyMsg.Set("(ututa(sssa{ss}))", nodeList.size(), limit, limit, offset, nodeList.size(), &nodeList.front());

    if (ER_OK != status) {
        QCC_LogError(status, ("ContentServerBusObject::ReplySearchMsgArgs: replyMsg.Set failed"));
        MethodReply(msg, status);
        return;
    }

    status = MethodReply(msg, &replyMsg, 1);
    if (ER_OK != status) {
        QCC_LogError(status, ("ContentServerBusObject::ReplySearchMsgArgs: MethodReply failed"));
    }
}

CapabilitiesMap* ContentServerBusObject::Capabilities(size_t capabilitiesCount, MsgArg* capablities)
{
    QCC_DbgTrace(("ContentServerBusObject::Capabilities"));
    if (capabilitiesCount == 0 || capablities == NULL) {
        return NULL;
    }

    //now populate the list to sort by
    CapabilitiesMap* capabilitiesMap = new CapabilitiesMap();
    for (size_t i = 0; i < capabilitiesCount; i++) {
        char* val = NULL;
        uint16_t order;

        QStatus status = capablities[i].Get("{ss}", &(val), &(order));
        if (status == ER_OK) {
            QCC_DbgTrace(("ContentServerBusObject::Capabilities sortBy=%s, order=%d", val, order));
            (*capabilitiesMap)[val] = order;
        } else {
            QCC_LogError(status, ("ContentServerBusObject::Capabilities: capablities[i].Get failed"));
        }
    }
    return capabilitiesMap;
}


void ContentServerBusObject::Browse(const InterfaceDescription::Member* member, Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::Browse"));

    const char* itemID = NULL;
    size_t capabilitiesCount = 0;
    MsgArg* capablities;
    uint64_t offset = 0;
    uint32_t limit = 0;
    size_t sortByCount = 0;
    MsgArg* sortBy = NULL;
    MsgArg outArgs;
    String itemIDString;

    QCC_DbgTrace(("ContentServerBusObject::Browse GetArgs"));
    QStatus status = msg->GetArgs("sa{ss}tia{si}", &itemID, &capabilitiesCount, &capablities, &offset, &limit, &sortByCount, &sortBy);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::Browse: msg->GetArgs Failed"));
        MethodReply(msg, status);
        return;
    }

    if (itemID == NULL) {
        QCC_LogError(status, ("ContentServerBusObject::Browse: missing itemID"));
        MethodReply(msg, ER_INVALID_DATA);
        return;
    }

    QCC_DbgTrace(("ContentServerBusObject::Browse itemId=%s", itemID));
    itemIDString = String(itemID);

    //now populate the list to sort by
    SortByMap sortByMap;
    for (size_t i = 0; i < sortByCount; i++) {
        char* val = NULL;
        uint16_t order;

        sortBy[i].Get("(sy)", &val, &order);
        if (val == NULL) {
            QCC_DbgTrace(("The sortBy is empty"));
        } else {
            QCC_DbgTrace(("ContentServerBusObject::Browse sortBy=%s, order=%d", val, order));
            sortByMap[val] = order;
        }
    }

    QCC_DbgTrace(("ContentServerBusObject::Browse Capabilities"));
    CapabilitiesMap* capabilitiesMap = Capabilities(capabilitiesCount, capablities);

    QCC_DbgTrace(("ContentServerBusObject::Browse m_ContentSource->Browse"));
    NodeList* nodes = m_ContentSource->Browse(itemID, *capabilitiesMap, offset, limit, sortByMap);

    QCC_DbgTrace(("ContentServerBusObject::Browse ReplySearchMsgArgs"));
    ReplySearchMsgArgs(msg, nodes, offset, limit);
}

void ContentServerBusObject::GetItem(const InterfaceDescription::Member* member, Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::Called GetItem"));

    char* itemID = NULL;

    QStatus status = msg->GetArgs("s", &itemID);
    if (status != ER_OK || itemID == NULL) {
        QCC_LogError(status, ("ContentServerBusObject::GetItem msg->GetArgs failed"));
        return;
    }

    if (itemID == NULL) {
        QCC_LogError(status, ("ContentServerBusObject::GetItem msg->GetArgs failed"));
        MethodReply(msg, status);
        return;
    }

    QCC_DbgTrace(("ContentServerBusObject::GetItem itemID=%s", itemID));

    Node* itemNode = m_ContentSource->GetItem(itemID);

    if (itemNode == NULL) {
        QCC_LogError(status, ("ContentServerBusObject::GetItem itemNode==NULL"));
        MethodReply(msg, ER_INVALID_DATA);
        return;
    }

    /*
       This is what the return value should look like

       (sssa{sv})
       s - itemID
       s - title
       s - type
       a{sv} : Arbitrary properties
       s - key
       v - value
     */

    PropertiesMap& propertiesMap = itemNode->GetProperties();

    QCC_DbgTrace(("ContentServerBusObject::GetItem GetProperties"));
    int index = 0;
    MsgArgList propertiesList(propertiesMap.size());
    for (PropertiesMap::iterator it = propertiesMap.begin(); it != propertiesMap.end(); ++it) {
        propertiesList.at(index++).Set("{sv}", it->first.c_str(), it->second.c_str());
    }

    MsgArg retMsgArg;

    status = retMsgArg.Set("(sssa{sv})", itemNode->GetItemID().c_str(), itemNode->GetTitle().c_str(), itemNode->GetTypeAsString().c_str(), propertiesList.size(), &propertiesList.front());
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::GetItem: retValue failed in set method"));
        MethodReply(msg, status);
        return;
    }

    QCC_DbgTrace(("ContentServerBusObject::GetItem retMsgArg"));
    status = MethodReply(msg, &retMsgArg, 1);

    if (ER_OK != status) {
        QCC_LogError(status, ("ContentServerBusObject::GetItem: Method reply failed"));
    }
}

void ContentServerBusObject::GetSortableFields(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::GetSortableFields"));

    SortableFieldsList* fields = m_ContentSource->GetSortableFields();

    int index = 0;
    MsgArgList fieldList(fields->size());
    for (SortableFieldsList::iterator it = fields->begin(); it != fields->end(); ++it) {
        QCC_DbgTrace(("ContentServerBusObject::GetSortableFields field=%s", it->c_str()));
        fieldList.at(index++).Set("(s)", it->c_str());
    }

    MsgArg msgArg;

    QCC_DbgTrace(("ContentServerBusObject::GetSortableFields: msgArg->Set"));
    QStatus status = msgArg.Set("a(s)", fieldList.size(), &fieldList.front());

    if (status != ER_OK) {
        QCC_LogError(status, ("retValue failed in set method"));
        MethodReply(msg, status);
        return;
    }

    QCC_DbgTrace(("ContentServerBusObject::GetSortableFields: MethodReply"));
    status = MethodReply(msg, &msgArg, 1);

    if (ER_OK != status) {
        QCC_LogError(status, ("Method reply failed"));
    }
}

void ContentServerBusObject::GetContentSourceInfo(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::GetContentSourceInfo"));

    SourceInfoMap* sourceInfoMap = m_ContentSource->GetContentSourceInfo();

    int index = 0;
    MsgArgList sourceInfoList(sourceInfoMap->size());
    for (SourceInfoMap::iterator it = sourceInfoMap->begin(); it != sourceInfoMap->end(); ++it) {
        QCC_DbgTrace(("ContentServerBusObject::GetContentSourceInfo key=%s, value=%s", it->first.c_str(), it->second.c_str()));
        sourceInfoList.at(index++).Set("{ss}", it->first.c_str(), it->second.c_str());
    }

    /*
       This is what the return value should look like

       a{sv} : Arbitrary properties
       s - key
       v - value
     */

    MsgArg msgArg;
    QCC_DbgTrace(("ContentServerBusObject::GetContentSourceInfo: msgArg.Set"));
    QStatus status = msgArg.Set("a{ss}", sourceInfoList.size(), &sourceInfoList.front());

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::GetContentSourceInfo Set failed"));
        MethodReply(msg, status);
        return;
    }
    status = MethodReply(msg, &msgArg, 1);

    if (ER_OK != status) {
        QCC_LogError(status, ("ContentServerBusObject::GetContentSourceInfo: reply failed"));
    }
}

void ContentServerBusObject::ModifyOptionalMetaData(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::ModifyOptionalMetaData"));

    const char* itemID = NULL;
    size_t metadataCount = 0;
    MsgArg* metadata;

    QStatus status = msg->GetArgs("sa{ss}", &itemID, &metadataCount, &metadata);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::ModifyOptionalMetaData: msg->GetArgs Failed"));
        MethodReply(msg, status);
        return;
    }

    if (itemID == NULL) {
        QCC_LogError(status, ("ContentServerBusObject::ModifyOptionalMetaData: msg->GetArgs Failed"));
        MethodReply(msg, ER_INVALID_DATA);
        return;
    }

    QCC_DbgTrace(("Request to alter item metadata with ID: %s", itemID));

    //now populate the list to sort by
    PropertiesMap propertiesMap;
    for (size_t i = 0; i < metadataCount; i++) {
        char* key = NULL;
        char* val = NULL;

        metadata[i].Get("{ss}", &key, &val);
        if (val != NULL) {
            QCC_DbgTrace(("ContentServerBusObject::ModifyOptionalMetaData key=%s, val=%s", key, val));
            propertiesMap[key] = val;
        }
    }

    m_ContentSource->ModifyOptionalMetaData(itemID, propertiesMap);

    MethodReply(msg);
}

void ContentServerBusObject::Search(const ajn::InterfaceDescription::Member* member, ajn::Message& msg)
{
    QCC_DbgTrace(("ContentServerBusObject::Search"));

    char* search = NULL;
    uint16_t searchMode = 0;
    size_t capabilitiesCount = 0;
    MsgArg* capablities;
    uint64_t offset = 0;
    uint32_t limit = 0;
    size_t sortByCount = 0;
    MsgArg* sortBy = NULL;
    MsgArg outArgs;
    String searchString;

    QStatus status = msg->GetArgs("sqa{ss}tia(si)", &search, &searchMode, &capabilitiesCount, &capablities, &offset, &limit, &sortByCount, &sortBy);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentServerBusObject::Search: msg->GetArgs Failed"));
        MethodReply(msg, status);
        return;
    }

    if (search != NULL) {
        QCC_DbgTrace(("ContentServerBusObject::Search=%s", search));
        searchString = String(search);
    }

    //now populate the list to sort by
    SortByMap sortByMap;
    for (size_t i = 0; i < sortByCount; i++) {
        char* val = NULL;
        uint16_t order;

        sortBy[i].Get("(sy)", &val, &order);
        if (val == NULL) {
            QCC_DbgTrace(("The sortBy is empty"));
        } else {
            QCC_DbgTrace(("ContentServerBusObject::Browse sortBy=%s, order=%d", val, order));
            sortByMap[val] = order;
        }
    }

    QCC_DbgTrace(("ContentServerBusObject::Browse Capabilities"));
    CapabilitiesMap* capabilitiesMap = Capabilities(capabilitiesCount, capablities);

    NodeList* nodes = m_ContentSource->Search(searchString, searchMode, *capabilitiesMap, offset, limit, sortByMap);

    ReplySearchMsgArgs(msg, nodes, offset, limit);
}

//override the base implementation to return real property values
QStatus ContentServerBusObject::Get(const char* ifcName, const char* propName, MsgArg& val)
{
    QStatus status = ER_OK;

    qcc::String propertyName(propName);
    QCC_DbgTrace(("ContentServerBusObject::Get propName=%s", propName));

    if (propertyName.compare(s_PROPERTY_VERSION) == 0) {
        QCC_DbgTrace(("ContentServerBusObject::Get propName=%s version=%d", propName, m_ContentSource->GetVersion()));
        val.Set("q", m_ContentSource->GetVersion());
    } else if (propertyName.compare(s_PROPERTY_DISPLAYNAME) == 0) {
        val.Set("s", m_ContentSource->GetDisplayName().c_str());
    } else if (propertyName.compare(s_PROPERTY_ICONURL) == 0) {
        val.Set("s", m_ContentSource->GetIconURL().c_str());
    } else if (propertyName.compare(s_PROPERTY_LASTMODIFIED) == 0) {
        val.Set("i", m_ContentSource->GetLastModifiedTime());
    } else {
        status = ER_BUS_NO_SUCH_PROPERTY;
        QCC_LogError(status, ("The property %s does is not defined for a content source", propertyName.c_str()));
    }

    return status;
}


} /* namespace mdf */
