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

#include "SampleContentSource.h"
#include <ContentConstants.h>
#include <stdio.h>
#include <stdlib.h>
#include <LogModule.h>
#include <alljoyn/Status.h>
#include "ContentManager.h"

namespace mdf {

const static qcc::String s_RootDirectory = "0";

SampleContentSource::SampleContentSource()
{
    Init();
}

SampleContentSource::~SampleContentSource() {
    delete m_ContentManager;
}

void SampleContentSource::Init()
{
    String directoryStructure = "{a,v,i,{a,{a,v}}}";
    m_ContentManager = new ContentManager(directoryStructure);

    m_Version = 142;
    m_DisplayName  = "SampleContentSourceDisplay";
    m_IconURL = "http://testUrl/c";
    m_LastModified = 1234;

    m_SortableFields.push_back("SortableField-1");
    m_SortableFields.push_back("SortableField-2");
    m_SortableFields.push_back("SortableField-3");

    m_SourceInfoMap["SourceInfo-key-1"] = "SourceInfo-value-1";
    m_SourceInfoMap["SourceInfo-key-2"] = "SourceInfo-value-2";
    m_SourceInfoMap["SourceInfo-key-3"] = "SourceInfo-value-3";
}

uint16_t SampleContentSource::GetVersion() const
{
    return m_Version;
}

String SampleContentSource::GetDisplayName() const
{
    return m_DisplayName;
}

qcc::String SampleContentSource::GetIconURL() const
{
    return m_IconURL;
}


uint32_t SampleContentSource::GetLastModifiedTime() const
{
    return m_LastModified;
}

NodeList* SampleContentSource::Browse(String itemID,
                                      CapabilitiesMap& playerCapabilities,
                                      uint64_t offset,
                                      uint32_t limit,
                                      SortByMap& sortByMap)
{
    QCC_DbgHLPrintf(("SampleContentSource::Browse"));

    QCC_DbgHLPrintf(("SampleContentSource::Browse itemId=%s", itemID.c_str()));
    NodeList* nodes = m_ContentManager->Browse(itemID, playerCapabilities, offset, limit, sortByMap);

    if (nodes->size() == 0) {
        QCC_DbgHLPrintf(("The node list returned from the query was empty"));
    }

    return nodes;
}

Node* SampleContentSource::GetItem(const qcc::String& itemID) const
{
    Node* node = m_ContentManager->GetItem(itemID);
    if (node == NULL) {
        return NULL;
    }

    return new Node(*node);
}

SortableFieldsList* SampleContentSource::GetSortableFields() {

    return &m_SortableFields;
}

SourceInfoMap* SampleContentSource::GetContentSourceInfo()  {

    return &m_SourceInfoMap;
}

QStatus SampleContentSource::ModifyOptionalMetaData(const qcc::String& nodeID, PropertiesMap& properties) {
    QStatus retVal = ER_OK;
    return retVal;
}

NodeList* SampleContentSource::Search(String& searchString, uint16_t searchMode, CapabilitiesMap& capabilities, uint64_t offset, uint32_t limit, SortByMap& sortByMap)
{
    QCC_DbgHLPrintf(("SampleContentSource::Browse search=%s", searchString.c_str()));
    NodeList* nodes = m_ContentManager->Search(searchString, searchMode, capabilities, offset, limit, sortByMap);

    if (nodes->size() == 0) {
        QCC_DbgHLPrintf(("The node list returned from the query was empty"));
    }
    return nodes;
}

} //mdf
