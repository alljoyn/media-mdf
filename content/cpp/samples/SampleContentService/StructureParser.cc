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

#include "StructureParser.h"
#include "StringUtils.h"
#include <algorithm>
#include <LogModule.h>
#include <alljoyn/Status.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstring>

#include "Node.h"


namespace mdf {

static const qcc::String s_STARTBRACE = "{";
static const qcc::String s_ENDBRACE = "}";
static const qcc::String s_AUDIO_FILE = "a";
static const qcc::String s_VIDEO_FILE = "v";
static const qcc::String s_IMAGE_FILE = "i";

StructureParser::StructureParser(const qcc::String& directoryStructure) : m_DirectoryStructure(directoryStructure)
{

}

StructureParser::~StructureParser()
{
    DeleteAllNodes();
}

void StructureParser::StartParsing()
{
    if (!IsDir(m_DirectoryStructure)) {
        QCC_LogError(ER_PARSE_ERROR, ("Directory structure must be enclosed in {}"));
        return;
    }

    m_NodeList.push_back(new Node("0", "ROOT", NodeTypeFolder));
}


void StructureParser::ParseDirRecursive(const qcc::String& directory,
                                        const qcc::String& parentID,
                                        std::vector<mdf::Node*>& nodeList)
{

    QCC_DbgHLPrintf(("ParseDirRecursive: Directory structure is %s and parent id is %s", directory.c_str(), parentID.c_str()));

    int curNumberOfSiblingFolders = 0;
    int curNumberOfSiblingFiles = 0;

    qcc::String tokenizeString = StripFolderBrackets(directory);

    std::vector<qcc::String> folderItems = SplitString(tokenizeString, ",");

    for (size_t i = 0; i < folderItems.size(); i++) {
        qcc::String folderItem = folderItems.at(i);

        if (IsDir(folderItem)) {
            curNumberOfSiblingFolders++;
            String newFolderName = CreateNewName(parentID, StringUtils::NumberToString(curNumberOfSiblingFolders));
            QCC_DbgHLPrintf(("Found folder. New Folder name is %s", newFolderName.c_str()));

            nodeList.push_back(new Node(newFolderName, newFolderName, NodeTypeFolder));
            ParseDirRecursive(folderItem, newFolderName, nodeList);

        } else if (folderItem.compare(s_AUDIO_FILE) == 0) {
            curNumberOfSiblingFiles++;
            String newFileName = CreateNewName(parentID, StringUtils::NumberToString(curNumberOfSiblingFiles)) + +".audio";
            QCC_DbgHLPrintf(("Found audio file. New file name is %s", newFileName.c_str()));

            nodeList.push_back(new Node(newFileName, newFileName, NodeTypeAudio));

        } else if (folderItem.compare(s_VIDEO_FILE) == 0) {
            curNumberOfSiblingFiles++;
            String newFileName = CreateNewName(parentID, StringUtils::NumberToString(curNumberOfSiblingFiles)) + ".video";
            QCC_DbgHLPrintf(("Found video file. New file name is %s", newFileName.c_str()));

            nodeList.push_back(new Node(newFileName, newFileName, NodeTypeAudio));

        } else if (folderItem.compare(s_IMAGE_FILE) == 0) {
            curNumberOfSiblingFiles++;
            String newFileName = CreateNewName(parentID, StringUtils::NumberToString(curNumberOfSiblingFiles)) + ".video";
            QCC_DbgHLPrintf(("Found video file. New file name is %s", newFileName.c_str()));

            nodeList.push_back(new Node(newFileName, newFileName, NodeTypeImage));
        }
    }
}

bool StructureParser::IsDir(const qcc::String& dir) const
{
    bool retVal = false;

    if (dir.find_first_of(*s_STARTBRACE.c_str()) == 0
        && dir.find_last_of(*s_ENDBRACE.c_str()) == (dir.size() - 1)) {
        retVal = true;
    }
    return retVal;
}


void StructureParser::DeleteNodePointer(Node* node)
{
    delete node;
    node = NULL;
}

void StructureParser::DeleteAllNodes()
{
    std::for_each(m_NodeList.begin(), m_NodeList.end(), std::bind1st(std::mem_fun(&StructureParser::DeleteNodePointer), this));
    m_NodeList.clear();
}

std::vector<qcc::String> StructureParser::SplitString(const qcc::String& stringToParse, const qcc::String& delimiter) {
    std::vector<qcc::String> retVal;

    /*size_t lastPos = stringToParse.find_first_not_of(delimiter.c_str(), 0);
       size_t pos = stringToParse.find_first_of(delimiter.c_str(), lastPos);

       while (qcc::String::npos != pos || qcc::String::npos != lastPos) {
        retVal.push_back(stringToParse.substr(lastPos, pos - lastPos));
        QCC_DbgHLPrintf(("Adding to the split array %s", stringToParse.substr(lastPos, pos - lastPos).c_str()));
        lastPos = stringToParse.find_first_not_of(delimiter.c_str(), pos);
        pos = stringToParse.find_first_of(delimiter.c_str(), lastPos);
       }
     */

    size_t pos = 0;
    while (pos < stringToParse.size()) {
        if (stringToParse[pos] == ',') {

            pos++;
        } else if (stringToParse[pos] == '{') {
            //first item is a folder structure
            size_t startPosOfFolder = pos;

            if (pos + 1 < stringToParse.size()) {
                //now we need to find where the folder structure ends
                int balance = 1;

                for (pos++; pos < stringToParse.size(); pos++) {
                    if (stringToParse[pos] == '{') {
                        balance++;
                    } else if (stringToParse[pos] == '}') {
                        balance++;
                    }

                    if (balance == 0) {
                        break;
                    }
                }
            }
            //now we have the position of the folder to extract
            retVal.push_back(stringToParse.substr(startPosOfFolder, pos - startPosOfFolder));
            QCC_DbgHLPrintf(("Adding to the split array %s", stringToParse.substr(startPosOfFolder, pos - startPosOfFolder).c_str()));

            //now increase the position for the next iteration of the while loop
            pos++;
        } else {
            //first item is a node = now extract the node
            size_t startPosOfFile = pos;
            pos = stringToParse.find_first_of(',', startPosOfFile);
            retVal.push_back(stringToParse.substr(startPosOfFile, pos - startPosOfFile));
            QCC_DbgHLPrintf(("Adding to the split array %s", stringToParse.substr(startPosOfFile, pos - startPosOfFile).c_str()));
        }
    }

    return retVal;

}

qcc::String StructureParser::StripFolderBrackets(const qcc::String& folder)
{
    //The assumption here is that this has already been verified to be a folder an that the braces exist
    qcc::String retVal;

    //a folder must be a minimum of '{}'
    if (folder.size() > 1) {
        retVal = folder.substr(1, folder.size() - 2);
        QCC_DbgHLPrintf(("StripFolderBrackets: Stripped brackets from the string %s and returned %s", folder.c_str(), retVal.c_str()));
    } else {
        QCC_LogError(ER_PARSE_ERROR, ("StripFolderBrackets: The folder %s does not have enough characters"));
    }
    return retVal;
}

String StructureParser::CreateNewName(const qcc::String& parentName, const qcc::String& indexInCurFolder) {
    String retVal;
    retVal = parentName + "." + indexInCurFolder;
    return retVal;
}

NodeList& StructureParser::GetNodeList()  {
    return m_NodeList;
}



} //end mdf
