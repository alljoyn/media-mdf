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

#include <ContentServiceClient.h>
#include <LogModule.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/about/AboutServiceApi.h>
#include <ContentConstants.h>
//#include "ContentAnnounceHandlerClient.h"
#include <iostream>
#include <sstream>
#include <ContentSourceListener.h>

using namespace ajn::services;

namespace mdf {

ContentServiceClient* ContentServiceClient::s_Instance(0);

/*
 * Non class member function pointer definition
 */

void AnnounceHandlerCallback(qcc::String const& busName, unsigned short version, unsigned short port,
                             const AnnounceHandler::ObjectDescriptions& objectDescs, const AnnounceHandler::AboutData& aboutData) {

    QCC_DbgHLPrintf(("Got announce callback - busName=%s, version=%d, port=%d\n", busName.c_str(), version, port));

    ContentServiceClient* contentServiceClientInstance = ContentServiceClient::GetInstance();
    ContentClientParams* params = contentServiceClientInstance->GetClientParams();
    BusAttachment* busAttachment = params->GetBusAttachment();
    QCC_DbgPrintf(("Our BusName  %s", busAttachment->GetUniqueName().c_str()));
    if (busAttachment && busAttachment->GetUniqueName().compare(busName) != 0) {
        QCC_DbgPrintf(("Found a content source on a different bus so adding it. BusName  %s", busName.c_str()));
        qcc::String contentSourcePrefix = s_PATH;
        AboutClient::ObjectDescriptions::const_iterator it;
        for (it = objectDescs.begin(); it != objectDescs.end(); ++it) {
            String key = it->first;

            if (key.compare(0, contentSourcePrefix.size(), contentSourcePrefix) == 0) {

                QCC_DbgHLPrintf(("Found a new content source on %s, key was: %s\n", busName.c_str(), key.c_str()));
                busAttachment->EnableConcurrentCallbacks();
                const SessionOpts* opts = params->GetSessionOpts();

                BusAttachmentParams* params = new BusAttachmentParams();
                params->m_BusAttachmentName = busName.c_str();
                params->m_BusAttachmentPath = key;
                void*vp = static_cast<void*>(params);

                QStatus status = busAttachment->JoinSessionAsync(busName.c_str(), (ajn::SessionPort) port,
                                                                 contentServiceClientInstance, *opts, contentServiceClientInstance, vp);

                if (status != ER_OK) {
                    QCC_LogError(status, ("There was a problem joining the session asynchronously"));
                }
            }

        }
    }   else {
        QCC_DbgPrintf(("This is an announcement from our bus so ignoring."));
    }
}

ContentServiceClient* ContentServiceClient::GetInstance() {
    if (!s_Instance) {
        s_Instance = new ContentServiceClient();
    }
    return s_Instance;
}

QStatus ContentServiceClient::Init(ContentClientParams* contentClientParams) {

    QStatus retVal = ER_OK;
//    if (m_Params == NULL) {
//        m_Params = contentClientParams;
//
//        if (m_Params != NULL) {
//            retVal = m_Params->Init();
//
//            if (retVal == ER_OK) {
//                m_ContentAnnounceHandlerClient = new ContentAnnounceHandlerClient( AnnounceHandlerCallback);
//                if (m_ContentAnnounceHandlerClient != NULL) {
//                    BusAttachment* busAttachment = m_Params->GetBusAttachment();
//                    if (busAttachment != NULL) {
//                        retVal = AnnouncementRegistrar::RegisterAnnounceHandler(*busAttachment, *m_ContentAnnounceHandlerClient);
//                        if (retVal == ER_OK) {
//                            retVal = busAttachment->AddMatch(s_ANNOUNCE_MATCH_RULE.c_str());
//                            if (retVal != ER_OK) {
//                                QCC_LogError(retVal, ("Could not add sessionless match."));
//                            }
//                        } else {
//                            retVal = ER_FAIL;
//                            QCC_LogError(retVal, ("The bus attachment returned from the contentserverparams is NULL"));
//                        }
//                    } else {
//                        retVal = ER_FAIL;
//                        QCC_LogError(retVal, ("Could not register announce handler."));
//                    }
//                } else {
//                    retVal = ER_INIT_FAILED;
//                    QCC_LogError(retVal, ("Could not initialise the announce handler"));
//                }
//            } else {
//                QCC_LogError(retVal, ("Content server params init failed"));
//            }
//        } else {
//            retVal = ER_BAD_ARG_1;
//            QCC_LogError(retVal, ("contentServer params cannot be NULL"));
//        }
//    } else {
//        retVal = ER_INIT_FAILED;
//        QCC_LogError(retVal, ("Server already initialised. Cannot initialise again"));
//    }

    return retVal;
}


ContentServiceClient::ContentServiceClient() :
    m_Params(NULL), m_ContentAnnounceHandlerClient(NULL)
{
}

ContentServiceClient::~ContentServiceClient() {

    delete m_Params;

    if (m_ContentAnnounceHandlerClient) {
        delete m_ContentAnnounceHandlerClient;
    }

    if (this == s_Instance) {
        s_Instance = NULL;
    }
}

ContentClientParams* ContentServiceClient::GetClientParams() {
    return m_Params;
}


QStatus ContentServiceClient::RegisterContentSourceListener(ContentSourceListener& contentSourceListener) {
    QStatus retVal = ER_OK;
    m_ContentSourceListeners.push_back(&contentSourceListener);
    return retVal;
}

QStatus ContentServiceClient::UnRegisterContentSourceListener(ContentSourceListener& contentSourceListener) {
    QStatus retVal = ER_OK;
    for (std::vector<ContentSourceListener*>::iterator it = m_ContentSourceListeners.end(); it != m_ContentSourceListeners.begin(); --it) {
        ContentSourceListener* listener = *it;
        if (listener == &contentSourceListener) {
            m_ContentSourceListeners.erase(it);
        }
    }
    return retVal;
}


void ContentServiceClient::Shutdown() {
    QCC_DbgHLPrintf(("Shutdown"));
    delete this;
}


uint16_t ContentServiceClient::GetVersion() const {
    return s_CONTENT_SERVICE_VERSION;
}

void ContentServiceClient::NewContentSourceFound(MdfContentSource* contentSource) {
    if (contentSource != NULL) {
        QCC_DbgHLPrintf(("Adding new content source"));
        m_ContentSourceList.push_back(contentSource);
        NotifyListenersOfNewContentSourceAdded(*contentSource);
    }
}

void ContentServiceClient::ContentSourceRemoved(uint32_t SessionId) {
}


void ContentServiceClient::NotifyListenersOfNewContentSourceAdded(MdfContentSource& contentSource)  {
    for (std::vector<ContentSourceListener*>::const_iterator it = m_ContentSourceListeners.begin(); it != m_ContentSourceListeners.end(); ++it) {
        ContentSourceListener* listener = *it;
        listener->ContentSourceAdded(contentSource);
    }
}


void ContentServiceClient::NotifyListenersOfNewContentSourceRemoved(MdfContentSource& contentSource) {
    for (std::vector<ContentSourceListener*>::const_iterator it = m_ContentSourceListeners.begin(); it != m_ContentSourceListeners.end(); ++it) {
        ContentSourceListener* listener = *it;
        listener->ContentSourceLost(contentSource);
    }
}


/*
 * Interface ajn::BusAttachment::JoinSessionAsyncCB
 */

void ContentServiceClient::JoinSessionCB(QStatus status, SessionId sessionId, const SessionOpts& opts, void* context) {
    if (status == ER_OK) {
        std::stringstream sessionIdStr;
        sessionIdStr << sessionId;
        QCC_DbgHLPrintf(("Joining session succeeded. SessionId:  %s", sessionIdStr.str().c_str()));

        BusAttachment* busAttachment =  m_Params->GetBusAttachment();
        busAttachment->EnableConcurrentCallbacks();
        busAttachment->SetSessionListener(sessionId, this);

        BusAttachmentParams* params = static_cast<BusAttachmentParams*>(context);

        ProxyBusObject* remoteObj = new ProxyBusObject(*busAttachment, params->m_BusAttachmentName.c_str(), params->m_BusAttachmentPath.c_str(), sessionId);

        delete params;

        const InterfaceDescription* alljoynTestIntf = busAttachment->GetInterface(s_CONTENT_SOURCE_INTERFACE.c_str());

        assert(alljoynTestIntf);
        remoteObj->AddInterface(*alljoynTestIntf);

        //BusContentSource* newContentSource = new BusContentSource(remoteObj, *busAttachment);

        // NewContentSourceFound(newContentSource);

    } else {
        QCC_LogError(status, ("Joining session failed."));
    }
}


/*
 * Interface ajn::SessionListener
 */
void ContentServiceClient::SessionLost(SessionId sessionId, SessionLostReason reason) {
    QCC_DbgHLPrintf(("Session Lost. SessionId:  %d", sessionId));
//    for (std::vector<MdfContentSource*>::iterator it = m_ContentSourceList.begin(); it != m_ContentSourceList.end(); it++) {
//        BusContentSource* src = (BusContentSource*)*it;
//
//        if (src->GetSessionID() == sessionId) {
//            //                  NotifyListenersOfNewContentSourceRemoved(*src);
//            m_ContentSourceList.erase(it);
//            QCC_DbgHLPrintf(("Removed"));
//            break;
//        }
//    }
}

} //Namespace ajn
