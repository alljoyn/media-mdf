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

#include <LogModule.h>
#include <qcc/Log.h>

#include "SampleContentServiceCalls.h"

namespace mdf {


SampleContentServiceCalls::SampleContentServiceCalls(ContentProxyBusObject* contentProxyBusObject) : m_ContentProxyBusObject(NULL)
{
    m_ContentProxyBusObject = contentProxyBusObject;
}

void SampleContentServiceCalls::SampleContentServiceCalls::Init()
{
}

void SampleContentServiceCalls::SampleContentServiceCalls::TestAllCalls()
{
    QCC_DbgTrace(("TestAllCalls"));

//    TestVersion();
//    TestDisplayName();
//    TestIconURL();
//    TestLastModified();

//  TestBrowse();
//    TestGetItem();
//    TestGetContentSourceInfo();
//    TestGetSortableFields();
//    TestModifyOptionalMetaData();
    TestSearch();
}

void SampleContentServiceCalls::TestVersion()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestVersion"));
    uint32_t version = m_ContentProxyBusObject->Version();
    QCC_DbgTrace((" SampleContentServiceCalls::TestVersion=%d", version));
}

void SampleContentServiceCalls::TestDisplayName()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestDisplayName"));
    String displayName = m_ContentProxyBusObject->DisplayName();
    QCC_DbgTrace((" SampleContentServiceCalls::TestDisplayName=%s", displayName.c_str()));
}
void SampleContentServiceCalls::TestIconURL()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestIconURL"));
    String iconurl = m_ContentProxyBusObject->IconURL();
    QCC_DbgTrace((" SampleContentServiceCalls::TestIconURL=%s", iconurl.c_str()));
}
void SampleContentServiceCalls::TestLastModified()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestLastModified"));
    uint32_t lastModified = m_ContentProxyBusObject->LastModified();
    QCC_DbgTrace((" SampleContentServiceCalls::TestLastModified=%ld", lastModified));
}

void SampleContentServiceCalls::SampleContentServiceCalls::TestBrowse()
{
    String itemId = "2";
    uint64_t offset = 10;
    uint32_t limit = 100;
    CapabilitiesMap capabilitiesMap;
    SortByMap sortByMap;

    capabilitiesMap["capKey1"] = 1;
    capabilitiesMap["capKey2"] = 2;
    capabilitiesMap["capKey3"] = 3;

    sortByMap["sortKey1"] = 1;
    sortByMap["sortKey2"] = 2;
    sortByMap["sortKey3"] = 3;

    QCC_DbgTrace(("SampleContentServiceCalls::TestBrowse item=%s, offset=%d, limit=%d", itemId.c_str(), offset, limit));

    NodeList* nodeList = m_ContentProxyBusObject->Browse(itemId, capabilitiesMap, offset, limit, sortByMap);

    PrintNodeList(nodeList);
}

void SampleContentServiceCalls::TestGetItem()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestGetItem"));
    String itemId = "2";

    Node* node = m_ContentProxyBusObject->GetItem(itemId);
    PrintNode(node);
}


void SampleContentServiceCalls::TestGetContentSourceInfo()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestGetContentSourceInfo"));
    SourceInfoMap* sourceInfoMap = m_ContentProxyBusObject->GetContentSourceInfo();

    for (SourceInfoMap::iterator it = sourceInfoMap->begin(); it != sourceInfoMap->end(); it++)
        QCC_DbgTrace((" SampleContentServiceCalls::TestGetContentSourceInfo sourceInfo key=%s, value=%s", it->first.c_str(), it->second.c_str()));
}

void SampleContentServiceCalls::TestGetSortableFields()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestGetSortableFields"));
    SortableFieldsList* sortableFieldsList = m_ContentProxyBusObject->GetSortableFields();

    for (SortableFieldsList::iterator it = sortableFieldsList->begin(); it != sortableFieldsList->end(); it++)
        QCC_DbgTrace((" SampleContentServiceCalls::TestGetSortableFields fiels=%s", it->c_str()));
}

void SampleContentServiceCalls::TestModifyOptionalMetaData()
{
    String itemId = "2";

    PropertiesMap propertiesMap;
    propertiesMap["MetaData-1"] = "MetaDataData-1";
    propertiesMap["MetaData-2"] = "MetaDataData-2";
    propertiesMap["MetaData-3"] = "MetaDataData-3";

    QCC_DbgTrace((" SampleContentServiceCalls::TestModifyOptionalMetaData"));
    m_ContentProxyBusObject->ModifyOptionalMetaData(itemId, propertiesMap);

}

void SampleContentServiceCalls::TestSearch()
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestSearch"));
    String search = "search";
    uint16_t searchMode = 2;
    CapabilitiesMap capabilitiesMap;
    SortByMap sortByMap;
    uint64_t offset = 10;
    uint32_t limit = 100;


    capabilitiesMap["capKey1"] = 1;
    capabilitiesMap["capKey2"] = 2;
    capabilitiesMap["capKey3"] = 3;


    NodeList* nodeList = m_ContentProxyBusObject->Search(search, searchMode, capabilitiesMap, offset, limit, sortByMap);

    PrintNodeList(nodeList);
}

void SampleContentServiceCalls::PrintNodeList(NodeList* nodeList)
{
    for (NodeList::iterator it = nodeList->begin(); it != nodeList->end(); it++)
        PrintNode(*it);
}

void SampleContentServiceCalls::PrintNode(Node* node)
{
    QCC_DbgTrace((" SampleContentServiceCalls::TestGetItem ItemId=%s, title=%s, type=%s, proptertiesCount=%d", node->GetItemID().c_str(), node->GetTitle().c_str(), node->GetTypeAsString().c_str(), node->GetProperties().size()));
}


} //mdf