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

#ifndef NODE_H_
#define NODE_H_

//#include <stdint.h>
#include <vector>

#include <qcc/String.h>
#include "ContentConstants.h"

using namespace qcc;

namespace mdf {

enum NodeType {
    NodeTypeFolder = 0,
    NodeTypeAudio = 100,
    NodeTypeVideo = 101,
    NodeTypeImage = 102,
    NodeTypeUnknown = 103
};


//node type also available as string
static const String s_Unknown = "UNKNOWN";
static const String s_FolderNode = "FOLDER";
static const String s_AudioNode = "AUDIO";
static const String s_ImageNode = "IMAGE";
static const String s_VideoNode = "VIDEO";


class Node {

  private:

    /*
     * @function Node
     * @description constructor
     */
    Node();

  public:
    /*
     * @function Node
     * @description constructor
     * @param itemID the id of the node
     * @param title The title of the node
     * @param nodeType Indicates if this is a folder, audio file, image file pr video file
     * @param properties custom properties
     */
    Node(const String itemID, const String title, NodeType nodeType, PropertiesMap& properties);

    Node(const String itemID, const String title, NodeType nodeType);

    /*
     * @function Node
     * @description constructor
     * @param itemID the id of the node
     * @param title The title of the node
     * @param nodeTypeAsString Indicates if this is a folder, audio file, image file pr video file
     * @param properties custom properties
     */
    Node(String itemID, String title, String type, PropertiesMap& properties);

    /*
     * @function ~Node
     * @description destructor
     */
    virtual ~Node();

    /*
     * @function GetItemID
     * @description returns the item ID
     * @return String the item ID
     */
    virtual String GetItemID() const;

    /*
     * @function SetItemID
     * @description sets the Item ID
     * @param itemID the item ID
     */
    virtual void SetItemID(const String& itemID);

    /*
     * @function GetItemID
     * @description returns the item ID. Note that parent ID is optional an not every node type will have a parent
     * @return String the item ID
     */
    virtual const String& GetParentID() const;

    /*
     * @function SetParentID
     * @description sets the optional parent ID
     * @param itemID the parent ID
     */
    virtual void SetParentID(const String& itemID);

    /*
     * @function GetTitle
     * @description gets the title
     * @return String the title
     */
    virtual const String& GetTitle() const;

    /*
     * @function SetTitle
     * @description sets the item title
     * @param title the title
     */
    virtual void SetTitle(const String& title);

    /*
     * @function GetType
     * @description gets the Item Type
     * @return NodeType the type
     */
    virtual NodeType GetType() const;

    /*
     * @function GetType
     * @description gets the Item Type as a string representation
     * @return NodeType the type. This can be one of "UNKNOWN", "FOLDER", "AUDIO", "IMAGE", "VIDEDO"
     */
    virtual const String& GetTypeAsString() const;

    /*
     * @function SetType
     * @description sets the item type
     * @param type the type
     */
    virtual void SetType(NodeType type);

    /*
     * @function SetType
     * @description sets the item type
     * @param type the type as a string
     */
    virtual void SetTypeFromString(const String& typeAsString);

    /*
     * @function GetProperties
     * @description returns a modifiable list of item properties
     */
    virtual PropertiesMap& GetProperties();

    /*
     * @function SetProperties
     * @description replaces the current properties object with the one supplied
     * @param properties the new properties
     */
    virtual void SetProperties(PropertiesMap& properties);


  protected:

    NodeType m_NodeType;        // the node type
    PropertiesMap m_Properties;        // custom properties

    String m_ItemID;     // the item id
    String m_Title;     // the item title
    String m_NodeTypeAsString; //the node type as a string
    String m_ParentID;     // the parent folcer id
};

} // mdf




#endif /* NODE_H_ */
