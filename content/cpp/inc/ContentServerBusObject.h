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

#ifndef CONTENTSERVERBUSOBJECT_H_
#define CONTENTSERVERBUSOBJECT_H_

#include <alljoyn/BusObject.h>
#include "MdfContentSource.h"

namespace mdf {

class ContentServerBusObject : public ajn::BusObject {

  public:

    /*
     * @function ContentServerBusObject
     * @description constructor
     * @param busObjectPath     object bus path
     * @param MdfContentSource  Source for all the data
     */
    ContentServerBusObject(const String& busObjectPath, MdfContentSource* contentSource);

    /*
     * @function ~ContentServerBusObject
     * @description destructor
     */
    virtual ~ContentServerBusObject();

    /*
     * @function Register
     * @descriptionRegisters the content source transport.
     * @returns ER_OK if successful, otherwise a range of errors.
     */
    QStatus Register();

    /*
     * @function GetContentSource
     * @description returns the content source
     * @return ContentSource* the content source
     */
    MdfContentSource* GetContentSource() { return m_ContentSource; };

  public: //from BusObject

    /*
     * @function Get
     * @description Handle a bus request to read a property from this object.
     * BusObjects that implement properties should override this method.
     *
     * @param ifcName    Identifies the interface that the property is defined on
     * @param propName  Identifies the the property to get
     * @param[out] val        Returns the property value. The type of this value is the actual value
     *                   type.
     * @return ER_OK if the property is retuned ok
     */
    virtual QStatus Get(const char* ifcName, const char* propName, MsgArg& val);

    QStatus CreateInterface(BusAttachment& busAttachment);

  private:

    QStatus CheckInterface(const InterfaceDescription* contentSourceInterface);

    QStatus AddContentMethodHandlers(BusAttachment& busAttachment);

    void ReplySearchMsgArgs(Message& msg, NodeList* nodes, uint64_t offset, uint32_t limit);

    CapabilitiesMap* Capabilities(size_t capabilitiesCount, MsgArg* capablities);


    /*
     * @function ObjectRegistered
     * @description called when an object is registered
     */
    void ObjectRegistered();

    /*
     * @function Browse
     * @discussion bus object implementation of the GetItem method\
     * @param member
     * @param msg
     */
    void Browse(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * @function GetItem
     * @discussion bus object implementation of the GetItem method\
     * @param member
     * @param msg
     */
    void GetItem(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * The busobject GetSortableFields method
     * @param member
     * @param msg
     */
    void GetSortableFields(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * The busobject GetContentSourceInfo method
     * @param member
     * @param msg
     */
    void GetContentSourceInfo(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * The busobject ModifyOptionalMetaData method
     * @param member
     * @param msg
     */
    void ModifyOptionalMetaData(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * The busobject Search method
     * @param member
     * @param msg
     */
    void Search(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

  private:

    MdfContentSource* m_ContentSource;
};

} /* namespace mdf */

#endif
