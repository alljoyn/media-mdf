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

#ifndef SampleContentSource_H
#define SampleContentSource_H

#include "MdfContentSource.h"
#include "CommonSampleUtil.h"
#include <Node.h>

namespace mdf {

using namespace ajn;
using namespace qcc;

class ContentManager;

class SampleContentSource : public MdfContentSource {

  public:
    /*
     * @function Constructor for the content source
     */
    SampleContentSource();

    /*
     * SampleContentSource destructor
     */
    ~SampleContentSource();

  public:

    /*
     * @function Init
     * @description This method is called by the Content server at Init time. If you have a lot of heavy processing to do as part of startup then this
     * is the place to do it by overriding this method
     */
    void Init();

    /*
     * @function GetVersion
     * @description  returns the version number
     * @return uint16_t
     */
    virtual uint16_t GetVersion() const;

    /*
     * @function GetIconURL
     * @description gets the icon url
     * @return qcc::String
     */
    virtual String GetIconURL() const;

    virtual String GetDisplayName() const;

    /*
     * @function GetLastModifiedTime
     * @description returns the last modified time
     * @return uint32_t the last modified time
     */
    virtual uint32_t GetLastModifiedTime() const;

    /*
     * @function Browse
     * @description a method that allows us to browse the database and return paginated results
     * @param itemID the ID of the folder to browse. Note that the value "0" should be used for the top level directory
     * @param playerCapabilities An array of key value pairs relating to the player capabilities. These values are used to filter the returned
     * response so that if an item cannot be played on the player the delivery URL is removed and if it can be played the relevant delivery URL is selected. If an empty array is submitted all returned items will have the delivery URLs removed and it is expected that the caller calls GetItem to obtain the delivery URLs for the item. Sample capability values: mimeTypes={“audio/mpeg,”video/mp4”} resolution=2560x1440
     * @param offset Use to specify offset within a result. The minimum offset is 0
     * @param limit The number of results to return. Default is 20
     * @param sortByMap Criteria for sorting the result. Call get sortable fields to find out which fields can be used for sorting the result. Each member of the the array is
     * a struct with a property defining the sort property and whether to sort ascending or descending
     * @return queryResponse a class containing the results of the browse request
     */
    virtual NodeList* Browse(String itemID, CapabilitiesMap& playerCapabilities, uint64_t offset, uint32_t limit,
                             SortByMap& sortByMap);

    /*
     * @function GetItem
     * @param itemID the id of the item to retrieve from the database
     */
    virtual Node* GetItem(const qcc::String& itemID) const;

    /*
     * @function GetSortableFields
     * @description returns An array of fields that can be used to sort the results returned by the content source e.g. artist, album, title etc.
     * @return std::vector<std::string> containing the names of the fields which can be used to sort
     */
    virtual SortableFieldsList* GetSortableFields();

    /*
     * @function GetContentSourceInfo
     * @description returns a map of key value pairs with information about the content source.
     * @return MsgArg a objwxt containing the key value pairs
     */
    virtual SourceInfoMap* GetContentSourceInfo();


    /*
     * @function ModifyOptionalMetaData
     * @description Allows you to modify the optional metadata of an item in the content source.
     * @param nodeID The ID of the item who’s metadata you want to change
     * @param properties An array of key value pairs. The key is the name of the property you want to change and the value the value.
     * The content source should validate this value to ensure it conforms to the relevant format.
     * @return QStatus indictaes if the operation was succesful, otherwise returns a range of errors
     */
    virtual QStatus ModifyOptionalMetaData(const qcc::String& nodeID, PropertiesMap& properties);


    /*
     * @function Search
     * @description used to search the content source for content
     * @param searchString the free text search String
     * @param searchMode The search mode to use.
     * Allowed Values:
     * 0 – Starts with. The fields must start with the submitted text.
     * 1 – Contains. The fields must contain the submitted text.
     * @param playerCapabilities An array of key value pairs relating to the player capabilities. These values are used to filter the returned response so that if an item cannot be played on the player the delivery URL is removed and if it can be played the relevant delivery URL is selected. If an empty array is submitted all returned items will have the delivery URLs removed and it is expected that the caller calls GetItem to obtain the delivery URLs for the item.
     * Sample capability values:
     * mimeTypes={“audio/mpeg,”video/mp4”}
     * resolution=2560x1440
     * @param offset Use to specify offset within a result. The minimum offset is 0.
     * @param limit The number of results to return. Default is 20.
     * @param sortByMap Criteria for sorting the result. Call GetSortableFields to find out which fields can be used for sorting.
     * {(“field”, 0|1), (“field”, 0|1),…}
     * e.g. {(“title”, 0)}. This field can be an empty array if order is not important.
     * 0 = ASC
     * 1 = DESC
     * @return queryResponse a class containing the results of the browse request
     */
    virtual NodeList* Search(String& searchString, uint16_t searchMode,
                             CapabilitiesMap& playerCapabilities, uint64_t offset,
                             uint32_t limit, SortByMap& sortByMap);

  private:

    uint16_t m_Version;
    String m_DisplayName;
    String m_IconURL;
    uint32_t m_LastModified;

    SortableFieldsList m_SortableFields;
    SourceInfoMap m_SourceInfoMap;

    ContentManager* m_ContentManager;


};

} // end mdf



#endif /* SampleContentSource_H */
