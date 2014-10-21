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

#ifndef CONTENTSERVICECLIENT_H_
#define CONTENTSERVICECLIENT_H_

#include "ContentClientParams.h"
#include <stdint.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/SessionListener.h>
#include <alljoyn/BusAttachment.h>
#include "MdfContentSource.h"
#include <map>
#include <vector>

namespace mdf {

//holder class to pass data to announcehandler callback
class BusAttachmentParams {
  public:
    qcc::String m_BusAttachmentName;
    qcc::String m_BusAttachmentPath;
};

class ContentAnnounceHandlerClient;

class ContentSourceListener;
/*
 * This class is used to represent a content service.
 */

class ContentServiceClient : public ajn::BusAttachment::JoinSessionAsyncCB, public ajn::SessionListener {

  public:

    /*
     * @function GetInstance
     * @description singleton constructor method. Only one instance of this should ever be running per machine. This returns the current
     * instance or creates a new one if none exists
     * @return a pointer to the ContentServiceClient instance
     */
    static ContentServiceClient* GetInstance();

    /*
     * @function ~ContentService
     * @description destructor
     */
    ~ContentServiceClient();

    QStatus CreateInterface(BusAttachment* busAttachment);

    /*
     * @function RegisterContentSourceListener
     * @description registers a content source listener
     * @param contentSourceListener a callback which is notified when a content source is added or removed
     * @return QStatus indictaes if the operation was succesful, otherwise returns a range of errors
     */
    QStatus RegisterContentSourceListener(ContentSourceListener& contentSourceListener);

    /*
     * @function UnRegisterContentSourceListener
     * @description removes a content source listener
     * @param contentSourceListener the listener to remove
     * @return QStatus indictaes if the operation was succesful, otherwise returns a range of errors
     */
    QStatus UnRegisterContentSourceListener(ContentSourceListener& contentSourceListener);

    /*
     * @function Init
     * @description Initialise the API.
     * @return ER_OK if initialised successfully, otherwise a range of errors
     */
    QStatus Init(ContentClientParams* contentClientParams);

    /*
     * @function Shutdown
     * @description Cleanup and get ready for shutdown.
     */
    void Shutdown();

    /*
     * @function GetVersion
     * @description Get the version
     * @return uint16_t the version
     */
    uint16_t GetVersion() const;

    /*
     * @function GetClientParams
     * @description returns the client params used to initalise the class
     * @return ContentClientParams* the client params used to initalise the class
     */
    ContentClientParams* GetClientParams();


  public:
    /*
     * @function JoinSessionCB
     * @description Called when JoinSessionAsync() completes.
     * @param status       ER_OK if successful
     * @param sessionId    Unique identifier for session.
     * @param opts         Session options.
     * @param context      User defined context which will be passed as-is to callback.
     */
    virtual void JoinSessionCB(QStatus status, SessionId sessionId, const SessionOpts& opts, void* context);

  public:
/*
 * @function SessionLost
 * @descriptionCalled by the bus when an existing session becomes disconnected.
 *
 * See also these sample file(s): @n
 * FileTransfer/FileTransferService.cc @n
 * simple/android/client/jni/Client_jni.cpp @n
 * simple/android/service/jni/Service_jni.cpp @n
 *
 * For Windows 8 see also these sample file(s): @n
 * cpp/Basic/Basic_Client/BasicClient/AllJoynObjects.cpp @n
 * cpp/Basic/Basic_Client/BasicClient/AllJoynObjects.h @n
 * cpp/Basic/Basic_Service/BasicService/AllJoynObjects.cpp @n
 * cpp/Basic/Basic_Service/BasicService/AllJoynObjects.h @n
 * cpp/Basic/Name_Change_Client/NameChangeClient/AllJoynObjects.cpp @n
 * cpp/Basic/Name_Change_Client/NameChangeClient/AllJoynObjects.h @n
 * cpp/Basic/Signal_Consumer_Client/SignalConsumerClient/AllJoynObjects.cpp @n
 * cpp/Basic/Signal_Consumer_Client/SignalConsumerClient/AllJoynObjects.h @n
 * cpp/Basic/Signal_Service/SignalService/AllJoynObjects.cpp @n
 * cpp/Basic/Signal_Service/SignalService/AllJoynObjects.h @n
 * cpp/Chat/Chat/AllJoynObjects.cpp @n
 * cpp/Chat/Chat/AllJoynObjects.h @n
 * cpp/Secure/Secure/AllJoynObjects.cpp @n
 * cpp/Secure/Secure/AllJoynObjects.h @n
 * csharp/blank/blank/Common/Listeners.cs @n
 * csharp/BusStress/BusStress/Common/ClientBusListener.cs @n
 * csharp/BusStress/BusStress/Common/ServiceBusListener.cs @n
 * csharp/chat/chat/Common/Listeners.cs @n
 * csharp/chat/chat/MainPage.xaml.cs @n
 * csharp/FileTransfer/Client/Common/Listeners.cs @n
 * csharp/Secure/Secure/Common/Listeners.cs @n
 * csharp/Sessions/Sessions/Common/MyBusListener.cs @n
 * csharp/Sessions/Sessions/Common/SessionOperations.cs @n
 *
 * @param sessionId     Id of session that was lost.
 * @param reason        The reason for the session being lost
 */
    virtual void SessionLost(SessionId sessionId, SessionLostReason reason);

  private:

    ContentServiceClient();  // Private so that it can not be called because this is a singleton
    ContentServiceClient(ContentServiceClient const&); // copy constructor is private not be called because this is a singleton
    ContentServiceClient& operator=(ContentServiceClient const&); // assignment operator is private not be called because this is a singleton

    /*
     * @function NewContentSourceFound
     * @description called when a new content source has been found
     * @param contentSource the new content source
     */
    void NewContentSourceFound(MdfContentSource* contentSource);

    /*
     * @function ContentSourceRemoved
     * @description called when a content source has been removed
     * @param SessionID the sessionID used in the contentSource
     */
    void ContentSourceRemoved(uint32_t SessionId);

    /*
     * @function NotifyListenersOfNewContentSourceAdded
     * @description notifies all listeners of a new content source added
     * @param contentSource the new content source added
     */
    void NotifyListenersOfNewContentSourceAdded(MdfContentSource& contentSource);

    /*
     * @function NotifyListenersOfNewContentSourceRemoved
     * @description notifies all listeners of a content source removed
     * @param contentSource the new content source removed
     */
    void NotifyListenersOfNewContentSourceRemoved(MdfContentSource& contentSource);


  private:

    static ContentServiceClient* s_Instance;

    uint16_t s_CONTENT_SERVICE_VERSION; //Version of the API

    std::vector<MdfContentSource*> m_ContentSourceList;     //vector of content source pointers - the pointers are owned by this class
    std::vector<ContentSourceListener*> m_ContentSourceListeners;     //vector of references to contentsourcelisteners

    ContentClientParams* m_Params;
    ContentAnnounceHandlerClient* m_ContentAnnounceHandlerClient;

};

} //namespace mdf


#endif /* CONTENTSERVICECLIENT_H_ */
