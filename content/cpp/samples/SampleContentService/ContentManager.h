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

#ifndef CONTENTMANAGER_H_
#define CONTENTMANAGER_H_

#include <qcc/String.h>
#include <vector>
#include <Node.h>
#include "MdfContentSource.h"
#include "StructureParser.h"

namespace mdf {

class ContentManager {

  public:

    /*
     * @function ContentManager
     * @description constructor
     */
    ContentManager(const qcc::String& directoryStructure);

    /*
     * @function ~ContentManager
     * @description destructor
     */
    virtual ~ContentManager();


    /*
     * @function BrowseDirectory
     * @description browse the directory whos ID is supplied
     * @param directoryID
     * @param sortByMap a vector of properties that we should sort the return values by
     * @param offset offset in the results to return
     * @param limit the limit is the max amount of results to return
     * @param supportedMimeTypeList a list of supported mime types
     * @param resolution resolution to be supported. 0 if not relevant
     * @param transportTypeList a list of transport types
     * @return std::vector<const mdf::Node> a list of Node pointers returned by the query
     */
    NodeList* Browse(String& itemID, CapabilitiesMap& playerCapabilities, uint64_t offset, uint32_t limit, SortByMap& sortBy);


    /*
     * @function GetItem
     * @param itemID the id of the item to return
     * @retVal Node* a pointer to a node item
     */
    Node* GetItem(const qcc::String& itemID) const;

    NodeList* Search(String search, uint16_t searchMode, CapabilitiesMap& playerCapabilities, uint64_t offset, uint32_t limit, SortByMap& sortBy);

  private:

    StructureParser* m_StructureParser;
};

} //end mdf

#endif /* ContentManager_H_ */
