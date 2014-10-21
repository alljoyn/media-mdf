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

#include <Node.h>
#include <LogModule.h>

using namespace qcc;

namespace mdf {

Node::Node() : m_NodeType(NodeTypeUnknown), m_NodeTypeAsString(s_Unknown) {

}

Node::Node(String itemID, String title, NodeType nodeType, PropertiesMap& properties) :
    m_ItemID(itemID), m_Title(title), m_Properties(properties)
{

    SetType(nodeType);
}

Node::Node(String itemID, String title, NodeType nodeType) :
    m_ItemID(itemID), m_Title(title)
{
    SetType(nodeType);
}

Node::Node(String itemID, String title, String type, PropertiesMap& properties) :
    m_ItemID(itemID), m_Title(title), m_Properties(properties) {
    SetTypeFromString(type);
}

Node::~Node() {

}

String Node::GetItemID() const {
    return m_ItemID;
}

void Node::SetItemID(const String& itemID) {
    m_ItemID = itemID;
}

const String& Node::GetParentID() const {
    return m_ParentID;
}

void Node::SetParentID(const String& itemID) {
    m_ParentID = itemID;
}

const String& Node::GetTitle() const {
    return m_Title;
}

void Node::SetTitle(const String& title) {
    m_Title = String(title.c_str());
}

NodeType Node::GetType() const {
    return m_NodeType;
}

const String& Node::GetTypeAsString() const {
    return m_NodeTypeAsString;
}

void Node::SetType(NodeType type) {

    m_NodeType = type;

    switch (type) {
    case NodeTypeFolder: {
            m_NodeTypeAsString = s_FolderNode;
        } break;

    case  NodeTypeAudio: {
            m_NodeTypeAsString = s_AudioNode;
        } break;

    case  NodeTypeImage: {
            m_NodeTypeAsString = s_ImageNode;
        } break;

    case  NodeTypeVideo: {
            m_NodeTypeAsString = s_VideoNode;
        } break;

    default: {
            m_NodeTypeAsString = s_FolderNode;
        } break;
    }
}

void Node::SetTypeFromString(const String& typeAsString) {

    m_NodeTypeAsString = typeAsString;

    if (typeAsString.compare(s_FolderNode) == 0) {
        m_NodeType = NodeTypeFolder;
    } else if (typeAsString.compare(s_AudioNode) == 0) {
        m_NodeType = NodeTypeAudio;
    } else if (typeAsString.compare(s_VideoNode) == 0) {
        m_NodeType = NodeTypeVideo;
    } else if (typeAsString.compare(s_ImageNode) == 0) {
        m_NodeType = NodeTypeImage;
    } else {
        m_NodeType = NodeTypeUnknown;
        m_NodeTypeAsString = s_Unknown;
    }

}

PropertiesMap& Node::GetProperties()  {
    return m_Properties;
}

void Node::SetProperties(PropertiesMap& properties) {
    m_Properties = properties;
}


} // mdf

