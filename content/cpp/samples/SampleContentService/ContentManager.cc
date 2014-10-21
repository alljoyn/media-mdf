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

#include "ContentManager.h"
#include <Node.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <LogModule.h>
#include <alljoyn/Status.h>
#include <ContentConstants.h>

namespace mdf {

ContentManager::ContentManager(const qcc::String& directoryStructure) : m_StructureParser(NULL)
{
    m_StructureParser = new StructureParser(directoryStructure);
    m_StructureParser->StartParsing();
}

ContentManager::~ContentManager()
{
    delete m_StructureParser;
}

NodeList* ContentManager::Browse(String& itemID, CapabilitiesMap& playerCapabilities, uint64_t offset, uint32_t limit, SortByMap& sortBy)
{

    QCC_DbgHLPrintf(("ContentManager::BrowseDirectory: itemID=%s", itemID.c_str()));

//    NodeList* nodeList = new NodeList;
    NodeList& allnodes = m_StructureParser->GetNodeList();
    return &allnodes;

//    for (size_t i = 0; i < allnodes.size(); i++) {
//        if (allnodes.at(i)->GetParentID().compare(itemID) == 0) {
//            nodeList->push_back(allnodes.at(i));
//            QCC_DbgHLPrintf(("Found node called - %s ", allnodes.at(i)->GetTitle().c_str()));
//        }
//    }
//
//    return nodeList;
}

Node* ContentManager::GetItem(const qcc::String& itemID) const
{
    QCC_DbgHLPrintf(("ContentManager::GetItem: Get item with id %s", itemID.c_str()));
//    Node* retVal = NULL;
//    const std::vector<mdf::Node*> allnodes = m_StructureParser->GetNodeList();
//
//    for (size_t i = 0; i < allnodes.size(); i++) {
//        if (allnodes.at(i)->GetItemID().compare(itemID) == 0) {
//            retVal = allnodes.at(i);
//            QCC_DbgHLPrintf(("Found node called - %s ", allnodes.at(i)->GetTitle().c_str()));
//            break;
//        }
//    }
//    return retVal;
    NodeList& allnodes = m_StructureParser->GetNodeList();
    return allnodes.at(0);
}

NodeList* ContentManager::Search(String search, uint16_t searchMode, CapabilitiesMap& playerCapabilities, uint64_t offset, uint32_t limit, SortByMap& sortBy)
{
    NodeList& allnodes = m_StructureParser->GetNodeList();
    return &allnodes;
}

} //end mdf
