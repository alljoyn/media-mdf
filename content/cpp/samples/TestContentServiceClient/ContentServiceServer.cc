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

#include <ContentServiceServer.h>
#include <LogModule.h>
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/about/AboutServiceApi.h>
#include "ContentConstants.h"
#include "ContentServiceSessionHandler.h"
//#include <iostream>
//#include <sstream>
#include <ContentSourceListener.h>


namespace mdf {

ContentServiceServer* ContentServiceServer::s_Instance(0);

ContentServiceServer* ContentServiceServer::GetInstance() {
    if (!s_Instance) {
        s_Instance = new ContentServiceServer();
    }
    return s_Instance;
}

ContentServiceServer::ContentServiceServer()
{
}

ContentServiceServer::~ContentServiceServer() {

//    if (m_ContentServerSessionHandler) {
//        delete m_ContentServerAnnounceHandler;
//        m_ContentServerAnnounceHandler = NULL;
//    }
//
//    if (m_ContentServerSessionHandler) {
//        delete m_ContentServerSessionHandler;
//        m_ContentServerSessionHandler = NULL;
//    }
//
//    if (this == s_Instance) {
//        s_Instance = 0;
//    }
}

void ContentServiceServer::Shutdown() {
    delete this;
}

uint16_t ContentServiceServer::GetServerVersion() const
{
    return s_CONTENT_SERVICE_VERSION;
}


static void AnnounceHandlerCallback(qcc::String const& busName, unsigned short version, unsigned short port,
                                    const ajn::services::AnnounceHandler::ObjectDescriptions& objectDescs, const ajn::services::AnnounceHandler::AboutData& aboutData)
{

    QCC_DbgPrintf(("Got announce callback - busName=%s, version=%d, port=%d", busName.c_str(), version, port));

    //SampleContentService* params = ContentServiceServer::GetInstance()->GetParams();
    //BusAttachment* busAttachment = params->GetBusAttachment(); //TODO:
    BusAttachment* busAttachment = NULL;

    if (busAttachment == NULL) {
        QCC_DbgPrintf(("busAttachment==NULL, so ignoring."));
        return;
    }

    if (busAttachment->GetUniqueName().compare(busName) == 0) {
        QCC_DbgPrintf(("This is an announcement from our bus so ignoring."));
        return;
    }

    QCC_DbgPrintf(("Found a content source on a different bus so adding i - BusName  %s", busName.c_str()));
    qcc::String contentSourcePrefix = s_PATH;

    ajn::services::AboutClient::ObjectDescriptions::const_iterator it;

    for (it = objectDescs.begin(); it != objectDescs.end(); ++it) {
        qcc::String key = it->first;
        std::vector<qcc::String> interfaces = it->second;

        if (key.compare(0, contentSourcePrefix.size(), contentSourcePrefix) == 0) {
            QCC_DbgPrintf(("New content busName=%s, key=%s", busName.c_str(), key.c_str()));

            busAttachment->EnableConcurrentCallbacks();

            //SessionOpts* opts = params->GetSessionOpts(); //TODO:
            SessionOpts* opts = NULL;

            static SessionId s_sessionId = 0;

            QStatus status = busAttachment->JoinSession(busName.c_str(), (ajn::SessionPort) port, ContentServiceServer::GetInstance()->GetContentServiceSessionHandler(), s_sessionId, *opts);
            if (status != ER_OK) {
                QCC_LogError(status, ("Failed to join session"));
            } else {
                QCC_DbgPrintf(("The session id is %d\n", s_sessionId));
                usleep(1000);

//                ProxyBusObject* remoteObj = new ProxyBusObject(*busAttachment, busName.c_str(), s_PATH.c_str(), s_sessionId);
//
//                const InterfaceDescription* alljoynTestIntf = busAttachment->GetInterface(s_CONTENT_SOURCE_INTERFACE.c_str());
//                if (alljoynTestIntf  != NULL) {
//                    remoteObj->AddInterface(*alljoynTestIntf);
//                } else {
//                    QCC_LogError(ER_INIT_FAILED, ("The interface description returned was null"));
//                }
            }
        }
    }
}

ContentServiceSessionHandler* ContentServiceServer::GetContentServiceSessionHandler()
{
    if (!m_ContentServerSessionHandler) {
        m_ContentServerSessionHandler = new ContentServiceSessionHandler();
    }

    return m_ContentServerSessionHandler;
}

QStatus ContentServiceServer::Init(ajn::BusAttachment* busAttachment)
{

    QStatus status = ER_OK;

//    status = ajn::services::AnnouncementRegistrar::RegisterAnnounceHandler(*busAttachment, this);
//    if (status != ER_OK) {
//        QCC_LogError(status, ("Could not register announce handler."));
//        return ER_INIT_FAILED;
//    }

    status = busAttachment->AddMatch(s_ANNOUNCE_MATCH_RULE.c_str());
    if (status != ER_OK) {
        QCC_LogError(status, ("Could not add sessionless match."));
        return ER_FAIL;
    }

    return ER_OK;
}

} //namespace mdf
