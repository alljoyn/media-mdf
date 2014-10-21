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

#ifndef STRUCTUREPARSER_H_
#define STRUCTUREPARSER_H_

#include <Node.h>
#include <qcc/String.h>
#include <vector>

namespace mdf {

class StructureParser {

  public:

    /*
     * @function StructureParser
     * @description constructor
     */
    StructureParser(const qcc::String& directoryStructure);

    /*
     * @function ~StructureParser
     * @description destructor
     */
    virtual ~StructureParser();

    /*
     * @function StartParsing
     * @description This kicks off the process of parsing the folder recursively for media, adding new media
     */
    void StartParsing();

    /*
     * @function GetNodeList
     * @description returns the list of nodes
     * @return std::vector<mdf::Node*>
     */
    NodeList& GetNodeList();

  private:

    /*
     * @function ParseDirRecursive
     * @description used to recursively parse folders and files
     * @param directory a representation of the directory to be parsed
     * @param parentID theParentID
     */
    void ParseDirRecursive(const qcc::String& directory, const qcc::String& parentID, std::vector<mdf::Node*>& m_NodeList);

    /*
     * @function IsDir
     * @description checks a file path to see if it is a directory
     * @return bool true if this is a directory, false if it is a file or does not exists
     */
    bool IsDir(const qcc::String& dir) const;

    /*
     * @function DeleteNodePointer
     * @description cleans up a node pointer
     * @param node the node to clean up
     */
    void DeleteNodePointer(Node* node);

    /*
     * @function DeleteAllNodes
     * @description deletes all nodes
     */
    void DeleteAllNodes();

    /*
     * @function SplitString
     * @description splits the token delimited string into individual strings
     * @return std::vector<qcc::String> the array of split strings
     */
    std::vector<qcc::String> SplitString(const qcc::String& stringToParse, const qcc::String& delimiter);

    /*
     * @function StripFolderBrackets
     * @description strips the enclosing braces from a folder structure
     * @param folder the folder string
     * @return qcc::String the folder with the braces removed
     */
    qcc::String StripFolderBrackets(const qcc::String& folder);

    /*
     * @folder CreateNewName
     * @description creates a new name using a combination of the parent folder ID and the items position in the current folder
     * @param parentName the parent folder name
     * @param indexInCurFolder
     * @return qcc::String the newly generated name
     */
    qcc::String CreateNewName(const qcc::String& parentName, const qcc::String& indexInCurFolder);

  private:

    String m_DirectoryStructure;
    NodeList m_NodeList;

};

} // end MDF

#endif /* StructureParser_H_ */
