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

#ifndef CONTENTCONSTANTS_H_
#define CONTENTCONSTANTS_H_

#include <map>
#include <vector>
#include <qcc/String.h>
#include "alljoyn/MsgArg.h"

/*
 * Common Macros
 */
#define CHECK(x) if ((status = x))  { break; }

using namespace qcc;
using namespace ajn;

namespace mdf {

class Node;

typedef std::map<String, uint16_t> SortByMap;
typedef std::map<String, String> CapabilitiesMap;
typedef std::map<String, String> SourceInfoMap;
typedef std::map<String, String> PropertiesMap;
typedef std::vector<Node*> NodeList;
typedef std::vector<String> SortableFieldsList;
typedef std::vector<MsgArg> MsgArgList;


static const String s_CONTENT_INTERFACE = "org.alljoyn.media.ContentSource";

static const String s_CONTENT_OBJECT_PATH = "/media/ContentSource";

static const String s_PROPERTY_VERSION = "version";
static const String s_PROPERTY_DISPLAYNAME = "displayName";
static const String s_PROPERTY_ICONURL = "iconURL";
static const String s_PROPERTY_LASTMODIFIED = "lastModified";

static const String s_SIGNAL_CONTENTSOURCEINDEXCHANGED = "ContentSourceIndexChanged";
static const String s_SIGNAL_METADATACHANGED = "MetaDataChanged";

static const String s_METHOD_BROWSE = "Browse";
static const String s_METHOD_GETITEM = "GetItem";
static const String s_METHOD_GETCONTENTSOURCEINFO = "GetContentSourceInfo";
static const String s_METHOD_GETSORTABLEFIELDS = "GetSortableFields";
static const String s_METHOD_MODIFYOPTIONALMETADATA = "ModifyOptionalMetaData";
static const String s_METHOD_SEARCH = "Search";


/** The aDudio interfaces XML. */
static const String s_CONTENT_SOURCE_IF_XML = "\
    <node name=\"\"> \
    <interface name=\"" + s_CONTENT_INTERFACE + "\">\
    <property  name=\"" + s_PROPERTY_VERSION         + "\" type=\"q\" access=\"read\"/>\
    <property  name=\"" + s_PROPERTY_DISPLAYNAME     + "\" type=\"s\" access=\"read\"/>\
    <property  name=\"" + s_PROPERTY_ICONURL         + "\" type=\"s\" access=\"read\"/>\
    <property  name=\"" + s_PROPERTY_LASTMODIFIED    + "\" type=\"i\" access=\"read\"/>\
    <method name=\"" + s_METHOD_BROWSE + "\">\
                        <arg name=\"itemID\" type=\"s\" direction=\"in\" />\
                        <arg name=\"playerCapabilities\" type=\"a{ss}\" direction=\"in\" />\
                        <arg name=\"offset\" type=\"t\" direction=\"in\" />\
                        <arg name=\"limit\" type=\"i\" direction=\"in\" />\
                        <arg name=\"sortBy\" type=\"a{si}\" direction=\"in\" />\
                  <arg name=\"response\" type=\"(ututa(sssa{ss}))\" direction=\"out\" />\
      </method>\
      <method name=\"" + s_METHOD_GETITEM + "\">\
              <arg name=\"itemID\" type=\"s\" direction=\"in\" />\
                  <arg name=\"response\" type=\"(sssa{sv})\" direction=\"out\" />\
      </method>\"\
        <method name=\"" + s_METHOD_GETSORTABLEFIELDS + "\">\
              <arg name=\"response\" type=\"a(s)\" direction=\"out\" />\
     </method>\
     <method name=\"" + s_METHOD_GETCONTENTSOURCEINFO + "\">\
              <arg name=\"response\" type=\"a{ss}\" direction=\"out\" />\
     </method>\
     <method name=\"" + s_METHOD_MODIFYOPTIONALMETADATA + "\">\
              <arg name=\"itemID\" type=\"s\" direction=\"in\" />\
              <arg name=\"properties\" type=\"a{ss}\" direction=\"in\" />\
    </method>\
    <method name=\"" + s_METHOD_SEARCH + "\">\
        <arg name=\"query\" type=\"s\" direction=\"in\" />\
        <arg name=\"searchMode\" type=\"q\" direction=\"in\" />\
        <arg name=\"playerCapabilities\" type=\"a{ss}\" direction=\"in\" />\
        <arg name=\"offset\" type=\"t\" direction=\"in\" />\
        <arg name=\"limit\" type=\"i\" direction=\"in\" />\
        <arg name=\"sortBy\" type=\"a(si)\" direction=\"in\" />\
        <arg name=\"response\" type=\"(ututa(sssa{ss}))\" direction=\"out\" />\
    </method>\
      <signal name=\"" + s_SIGNAL_CONTENTSOURCEINDEXCHANGED + "\">\
           <arg name=\"timestamp\" type=\"s\"/>\
      </signal>\
      <signal name=\"" + s_SIGNAL_METADATACHANGED + "\">\
            <arg name=\"timestamp\" type=\"s\"/>\
      </signal>\
    </interface> \
    </node>";

//custom properties that are returned by a node
static const String s_FILEURL = "FILEURL";

const uint16_t s_CONTENT_SERVICE_VERSION = 1;

/** The Announce interface name. */
static const String s_ABOUT_INTERFACE = "org.alljoyn.About";

/** The match rule for receiving Announce signals. */
const String s_ANNOUNCE_MATCH_RULE = "type='signal',interface='" + s_ABOUT_INTERFACE + "',member='Announce',sessionless='t'";

} //End mdf



#endif /* CONTENTCONSTANTS_H_ */
