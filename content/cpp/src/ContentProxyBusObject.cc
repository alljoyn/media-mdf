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

#include "ContentProxyBusObject.h"
#include <LogModule.h>

#include "Node.h"

#include "ContentConstants.h"

namespace mdf {

ContentProxyBusObject::ContentProxyBusObject(BusAttachment* busAttachment, String& serverBusName, SessionId sessionId)
{
    QCC_DbgTrace(("ContentProxyBusObject sessionId=%ld", sessionId));
    m_BusAttachment = busAttachment;
    m_SessionId = sessionId;
    m_ServerBusName = serverBusName;
    m_ProxyBusObject = NULL;
    Init();
}

void ContentProxyBusObject::Init()
{
    QStatus status = m_BusAttachment->CreateInterfacesFromXml(s_CONTENT_SOURCE_IF_XML.c_str());
    if (status != ER_OK) {
        QCC_LogError(ER_INIT_FAILED, ("m_BusAttachment->CreateInterfacesFromXml failed"));
        return;
    }

    const InterfaceDescription* interfaceDescription = m_BusAttachment->GetInterface(s_CONTENT_INTERFACE.c_str());
    if (interfaceDescription == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("The interface description returned was null"));
        return;
    }

    QCC_DbgTrace(("ContentProxyBusObject: Init interface=%s, path=%s,sessionId=%ld", s_CONTENT_INTERFACE.c_str(), s_CONTENT_OBJECT_PATH.c_str(), m_SessionId));

    m_ProxyBusObject = new ProxyBusObject(*m_BusAttachment, m_ServerBusName.c_str(), s_CONTENT_OBJECT_PATH.c_str(), m_SessionId);

    QCC_DbgTrace(("ContentProxyBusObject: Init:  m_ProxyBusObject->AddInterface"));
    m_ProxyBusObject->AddInterface(*interfaceDescription);
}

ContentProxyBusObject::~ContentProxyBusObject()
{
    if (m_ProxyBusObject) {
        delete m_ProxyBusObject;
    }
}

/*
   This is what the output arguments should look like:

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

NodeList* ContentProxyBusObject::CreateReplyNodeList(Message& reply)
{
    QCC_DbgTrace(("ContentProxyBusObject::CreateReplyNodeList"));

    uint32_t numResults;
    uint64_t maxResultsThatCanBeReturned;
    uint32_t maxResultsToReturn;
    uint64_t offSet;
    size_t numNodes;
    MsgArg* nodeData;

    QStatus status = reply->GetArgs("(ututa(sssa{ss}))", &numResults, &maxResultsThatCanBeReturned, &maxResultsToReturn, &offSet, &numNodes, &nodeData);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::CreateReplyNodeList: GetArgs Failed"));
        return NULL;
    }

    QCC_DbgTrace(("ContentProxyBusObject::CreateReplyNodeList resultCount=%d", numResults));
    NodeList* nodes = new NodeList;
    for (size_t i = 0; i < numNodes; i++) {
        char* itemID;
        char* title;
        char* type;
        size_t numArbitraryProps;
        MsgArg* arbitraryProps;

        status = nodeData[i].Get("(sssa{ss})", &itemID, &title, &type, &numArbitraryProps, &arbitraryProps);
        if (status != ER_OK) {
            QCC_LogError(status, ("getting individual node failed."));
            continue;
        }

        PropertiesMap properties;
        QStatus err; // = properties.Set("a{sv}", numArbitraryProps, arbitraryProps);
        if (err != ER_OK) {
            QCC_LogError(err, ("setting a custom property failed))"));
        }

        Node* node = CreateNode(itemID, title, type, properties);
        if (node) {
            nodes->push_back(node);
        }
    }
    return nodes;
}

NodeList* ContentProxyBusObject::Browse(String& itemID, CapabilitiesMap& capabilitiesMap, uint64_t offset, uint32_t limit, SortByMap& sortBy)
{
    QCC_DbgTrace(("ContentProxyBusObject::Browse"));

    Message reply(*m_BusAttachment);

    MsgArgList inputs(5);

    QCC_DbgTrace(("ContentProxyBusObject::Browse: SortByMap, count=%d", sortBy.size()));
    int index = 0;
    MsgArgList sortByList(sortBy.size());
    for (SortByMap::iterator it = sortBy.begin(); it != sortBy.end(); ++it) {
        sortByList.at(index++).Set("{si}", it->first.c_str(), it->second);
    }

    index = 0;
    QCC_DbgTrace(("ContentProxyBusObject::Browse: capabilitiesMap count=%d", capabilitiesMap.size()));
    MsgArgList capabilityList(capabilitiesMap.size());
    for (CapabilitiesMap::iterator it = capabilitiesMap.begin(); it != capabilitiesMap.end(); ++it) {
        capabilityList.at(index++).Set("{ss}", it->first.c_str(), it->second.c_str());
    }

    QCC_DbgTrace(("ContentProxyBusObject::Browse: set inputs"));
    inputs.at(0).Set("s", itemID.c_str());
    inputs.at(1).Set("a{ss}", capabilityList.size(), &capabilityList.front());
    inputs.at(1).Set("a{ss}", 0, NULL);
    inputs.at(2).Set("t", offset);
    inputs.at(3).Set("i", limit);
    inputs.at(4).Set("a{si}", sortByList.size(), &sortByList.front());

    /* =========================
       Make the call to the server
       ===========================*/
    QCC_DbgTrace(("ContentProxyBusObject::Browse m_ProxyBusObject->MethodCall"));
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_BROWSE.c_str(), &inputs.front(), inputs.size(), reply, 5000);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::Browse: MethodCall Failed"));
        return NULL;
    }

    QCC_DbgTrace(("ContentProxyBusObject::Browse CreateReplyNodeList"));
    return CreateReplyNodeList(reply);
}

Node* ContentProxyBusObject::GetItem(const String& itemID)
{
    QCC_DbgTrace(("ContentProxyBusObject::GetItem=%s", itemID.c_str()));

    /*
     * This is what the input value looks like
     * <arg name=\"itemID\" type=\"s\" direction=\"in\" />\
     */
    Message reply(*m_BusAttachment);
    MsgArgList inputs(1);

    inputs[0].Set("s", itemID.c_str());

    /* =========================
       Make the call to the server
       ===========================*/
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_GETITEM.c_str(), &inputs.front(), 1, reply, 5000);
    if (status != ER_OK) {
        QCC_LogError(status, ("getting individual node failed"));
        return NULL;
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

    char* title;
    char* type;
    size_t numArbitraryProps;
    MsgArg* arbitraryProps;

    status = reply->GetArgs("(sssa{sv})", &itemID, &title, &type, &numArbitraryProps, &arbitraryProps);
    if (status != ER_OK) {
        QCC_LogError(status, ("reply->GetArgs failed))"));
        return NULL;
    }

    PropertiesMap properties;
    //status = properties.Set("a{sv}", numArbitraryProps, arbitraryProps);
    if (status != ER_OK) {
        QCC_LogError(status, ("setting a custom property failed))"));
        return NULL;
    }

    return CreateNode(itemID, title, type, properties);
}

SortableFieldsList* ContentProxyBusObject::GetSortableFields()
{
    QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields"));

    SortableFieldsList* sortableFields = new SortableFieldsList;

    Message reply(*m_BusAttachment);

    QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields: ProxyBusObject->MethodCall"));
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_GETSORTABLEFIELDS.c_str(), NULL, 0, reply, 5000);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetSortableFields: MethodCall failed"));
        return NULL;
    }

    /*
       This is what the return value should look like
       as : array of strings
     */

    size_t numFields;
    MsgArg* fields;

    QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields: reply->GetArgs a(s)"));
    status = reply->GetArgs("a(s)", &numFields, &fields);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetSortableFields: setArgs failed))"));
        return NULL;
    }

    QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields: numFields=%d", numFields));

    for (size_t x = 0; x < numFields; x++) {
        char* field;

        QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields: sourceInfoMsgArgs.get (s)"));
        status = fields[x].Get("(s)", &field);
        if (status != ER_OK) {
            QCC_LogError(status, ("getting individual node failed."));
            continue;
        }

        QCC_DbgTrace(("ContentProxyBusObject::GetSortableFields field=%s", field));
        sortableFields->push_back(field);
    }

    return sortableFields;
}


SourceInfoMap* ContentProxyBusObject::GetContentSourceInfo()
{
    QCC_DbgTrace(("ContentProxyBusObject::GetContentSourceInfo"));

    Message reply(*m_BusAttachment);

    QCC_DbgTrace(("ContentProxyBusObject::GetContentSourceInfo: MethodCall"));
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_GETCONTENTSOURCEINFO.c_str(), NULL, 0, reply, 5000);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetContentSourceInfo MethodCall failed"));
        return NULL;
    }

    /*
       This is what the return value should look like

       a{sv} : Arbitrary properties
       s - key
       v - value
     */

    size_t sourceInfoCount;
    MsgArg* sourceInfoMsgArgs;

    QCC_DbgTrace(("ContentProxyBusObject::GetContentSourceInfo: reply->GetArgs"));
    status = reply->GetArgs("a{ss}", &sourceInfoCount, &sourceInfoMsgArgs);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetContentSourceInfo GetArgs failed"));
        return NULL;
    }

    QCC_DbgTrace(("ContentProxyBusObject::GetContentSourceInfo fieldCount=%d", sourceInfoCount));

    SourceInfoMap* sourceInfoMap = new SourceInfoMap;

    for (size_t x = 0; x < sourceInfoCount; x++) {
        char* infoKey;
        char* infoValue;

        status = sourceInfoMsgArgs[x].Get("{ss}", &infoKey, &infoValue);
        if (status != ER_OK) {
            QCC_LogError(status, ("getting individual node failed."));
            continue;
        }

        QCC_DbgTrace(("ContentProxyBusObject::GetContentSourceInfo key=%s value=%s", infoKey, infoValue));
        (*sourceInfoMap)[infoKey] = infoValue;
    }

    return sourceInfoMap;
}

QStatus ContentProxyBusObject::ModifyOptionalMetaData(String& itemID, PropertiesMap& propertiesMap)
{
    QCC_DbgTrace(("ContentProxyBusObject::ModifyOptionalMetaData itemID=%s", itemID.c_str()));

    Message reply(*m_BusAttachment);
    MsgArgList inputs(2);

    /*
       This is what the input arguments should look like:

       <arg name="itemID" type="s" direction="in" />
       <arg name="properties" type="a{sv}" direction="in" />
     */

    int index = 0;
    MsgArgList propertiesList(propertiesMap.size());
    for (PropertiesMap::iterator it = propertiesMap.begin(); it != propertiesMap.end(); ++it) {
        propertiesList.at(index++).Set("{ss}", it->first.c_str(), it->second.c_str());
    }

    inputs.at(0).Set("s", itemID.c_str());
    inputs.at(1).Set("a{ss}", propertiesList.size(), &propertiesList.front());

    QCC_DbgTrace(("ContentServerBusObject::ModifyOptionalMetaData: MethodCall"));
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_MODIFYOPTIONALMETADATA.c_str(), &inputs.front(), inputs.size(), reply, 5000);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::ModifyOptionalMetaData m_ProxyBusObject->MethodCall Failed"));
    }

    return status;
}


NodeList* ContentProxyBusObject::Search(String& searchString, uint16_t searchMode, CapabilitiesMap& capabilitiesMap, uint64_t offset, uint32_t limit, SortByMap& sortBy)
{
    QCC_DbgTrace(("ContentProxyBusObject::Search=%s, mode=%d", searchString.c_str(), searchMode));

    Message reply(*m_BusAttachment);

    QCC_DbgTrace(("ContentProxyBusObject::Browse: SortByMap, count=%d", sortBy.size()));
    int index = 0;
    MsgArgList sortByList(sortBy.size());
    for (SortByMap::iterator it = sortBy.begin(); it != sortBy.end(); ++it) {
        sortByList.at(index++).Set("{si}", it->first.c_str(), it->second);
    }

    index = 0;
    QCC_DbgTrace(("ContentProxyBusObject::Browse: capabilitiesMap count=%d", capabilitiesMap.size()));
    MsgArgList capabilityList(capabilitiesMap.size());
    for (CapabilitiesMap::iterator it = capabilitiesMap.begin(); it != capabilitiesMap.end(); ++it) {
        capabilityList.at(index++).Set("{ss}", it->first.c_str(), it->second.c_str());
    }

    MsgArgList inputs(6);
    inputs.at(0).Set("s", searchString.c_str());
    inputs.at(1).Set("q", searchMode);
    inputs.at(2).Set("a{ss}", capabilityList.size(), &capabilityList.front());
    inputs.at(3).Set("t", offset);
    inputs.at(4).Set("i", limit);
    inputs.at(5).Set("a(si)", sortByList.size(), &sortByList.front());

    /* =========================
       Make the call to the server
       ===========================*/
    QCC_DbgTrace(("ContentProxyBusObject::Search m_ProxyBusObject->MethodCall"));
    QStatus status = m_ProxyBusObject->MethodCall(s_CONTENT_INTERFACE.c_str(), s_METHOD_SEARCH.c_str(), &inputs.front(), inputs.size(), reply, 5000);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::Search: MethodCall Failed"));
        return NULL;
    }

    return CreateReplyNodeList(reply);
}


uint16_t ContentProxyBusObject::Version()
{
    QCC_DbgTrace(("ContentProxyBusObject Version"));
    // Message reply(*m_BusAttachment);
    MsgArg property;

    QStatus status = m_ProxyBusObject->GetProperty(s_CONTENT_INTERFACE.c_str(), s_PROPERTY_VERSION.c_str(), property);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::Version method call failed!"));
        return 0;
    }

    uint16_t version;
    status = property.Get("q", &version);
    if (status == ER_OK) {
        QCC_DbgTrace(("ContentProxyBusObject Version=%d", version));
    } else {
        QCC_LogError(status, ("ContentProxyBusObject::Version: Error extracting version"));
        return 0;
    }

    return version;
}

String ContentProxyBusObject::DisplayName()
{
    QCC_DbgTrace(("ContentProxyBusObject DisplayName"));
    MsgArg property;
    QStatus status = m_ProxyBusObject->GetProperty(s_CONTENT_INTERFACE.c_str(), s_PROPERTY_DISPLAYNAME.c_str(), property);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetDisplayName method call failed!"));
        return String("");
    }

    char* displayName;
    status = property.Get("s", &displayName);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::GetDisplayName: Error extracting displayName"));
        return String("");
    }

    return displayName;
}

String ContentProxyBusObject::IconURL()
{
    QCC_DbgTrace(("ContentProxyBusObject IconURL"));
    MsgArg property;
    QStatus status = m_ProxyBusObject->GetProperty(s_CONTENT_INTERFACE.c_str(), s_PROPERTY_ICONURL.c_str(), property);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::IconURL method call failed!"));
        return String("");
    }

    char* iconUrl;
    status = property.Get("s", &iconUrl);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::IconURL: Error extracting IconURL"));
        return String("");
    }

    return iconUrl;
}

uint32_t ContentProxyBusObject::LastModified()
{
    QCC_DbgTrace(("ContentProxyBusObject LastModified"));

    MsgArg property;
    QStatus status = m_ProxyBusObject->GetProperty(s_CONTENT_INTERFACE.c_str(), s_PROPERTY_LASTMODIFIED.c_str(), property);

    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::LastModified method call failed!"));
        return 0;
    }

    uint32_t lastModified = 0;
    status = property.Get("i", &lastModified);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentProxyBusObject::LastModified: Error extracting lastModified"));
        return 0;
    }
    return lastModified;
}

Node* ContentProxyBusObject::CreateNode(String itemID, String title, String type, PropertiesMap& properties)
{
    QCC_DbgTrace(("ContentProxyBusObject::CreateNode itemID=%s, title=%s, type=%s", itemID.c_str(), title.c_str(), type.c_str()));

    return new Node(itemID, title, type, properties);
}


} //END mdf
