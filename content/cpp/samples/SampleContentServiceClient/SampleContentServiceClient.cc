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

#include "SampleContentServiceClient.h"

#include "ContentBusListener.h"
#include "ContentSessionListenerClient.h"

#include "ContentBusListenerClient.h"

#include "ContentConstants.h"
#include "SampleContentServiceCalls.h"

#include <Node.h>
#include <LogModule.h>
#include <qcc/String.h>

#include "alljoyn/about/AnnouncementRegistrar.h"
#include "ContentSourceClient.h"
#include <alljoyn/BusAttachment.h>

namespace mdf {

SampleContentServiceClient::SampleContentServiceClient() :
    m_ClientSessionPort(0), m_BusAttachment(NULL), m_ClientSessionId(0)
{
}

SampleContentServiceClient::~SampleContentServiceClient()
{
    delete m_BusAttachment;
}

QStatus SampleContentServiceClient::Init()
{
    QCC_DbgTrace(("Init - started"));

    QStatus status  = ER_OK;

    QCC_DbgTrace(("Init - CreateBusAttachment"));
    m_BusAttachment = CreateBusAttachment();
    if (m_BusAttachment == NULL) {
        QCC_LogError(ER_OUT_OF_MEMORY, ("Create Bus attachment failed"));
        return ER_OUT_OF_MEMORY;
    }

    QCC_DbgTrace(("Init - m_BusAttachment->BusAttachment->Start"));
    status = m_BusAttachment->Start();
    if (status != ER_OK) {
        QCC_LogError(status, ("m_BusAttachment->Start failed"));
        return status;
    }

    QCC_DbgTrace(("Init - m_BusAttachment->Connect"));
    status = m_BusAttachment->Connect();
    if (status != ER_OK) {
        QCC_LogError(status, ("BusAttachment->Connect failed"));
        return status;
    }

    QCC_DbgTrace(("Init - m_BusAttachment busName=%s", m_BusAttachment->GetUniqueName().c_str()));

    m_ContentSessionListenerClient = new ContentSessionListenerClient();
    if (m_ContentSessionListenerClient == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("ContentSessionListenerClient failed"));
        return status;
    }

    m_ContentBusListenerClient = new ContentBusListenerClient(m_BusAttachment, m_ContentSessionListenerClient, m_ClientSessionPort, m_ClientSessionId);
    if (m_ContentBusListenerClient == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("ContentBusListenerClient failed"));
        return status;
    }

    //Used for advertised name,connect,disconnect
    QCC_DbgTrace(("Init - RegisterBusListener"));
    m_BusAttachment->RegisterBusListener(*m_ContentBusListenerClient);

    //Listening to announcements
    QCC_DbgTrace(("Init - ContentAnnounceHandlerServer"));

    //To get notifications the server has gone, we need to "findAdvertisedName" using ":" for all announcements or wait until the connected then start monitoring on the just for that connection.
    //Perhaps add code HERE???

    //This is for Client
    //Use about only if need to resolve the icon
    //Join session - gives the port.b
    //Can remove the proxy (can be asynchronously)

    const char* interfaces[1] = { s_ABOUT_INTERFACE.c_str() };

    QCC_DbgTrace(("Init - AnnouncementRegistrar::RegisterAnnounceHandler"));
    status = ajn::services::AnnouncementRegistrar::RegisterAnnounceHandler(*m_BusAttachment, *this, interfaces, 1);
    if (status != ER_OK) {
        QCC_LogError(ER_INIT_FAILED, ("AnnouncementRegistrar::RegisterAnnounceHandler failed"));
        return ER_INIT_FAILED;
    }

    //Only required by client to listen the server signal
    //ASK : Is this required for "About"
//    status = m_BusAttachment->AddMatch(s_ANNOUNCE_MATCH_RULE.c_str());
//    if (status != ER_OK)
//    {
//        QCC_LogError(ER_INIT_FAILED, ("BusAttachment->AddMatch failed"));
//        return ER_INIT_FAILED;
//    }

    QCC_DbgTrace(("Init - complete OK"));
    return ER_OK;
}

BusAttachment* SampleContentServiceClient::CreateBusAttachment()
{
    BusAttachment* busAttachment = new BusAttachment("CommonServiceApp", true);
    if (busAttachment == NULL) {
        QCC_LogError(ER_INIT_FAILED, ("could not initialise the bus attachment"));
        return NULL;
    }

    return busAttachment;
}


/** Wait for join session to complete, report the event to stdout, and return the result status. */
QStatus SampleContentServiceClient::WaitForJoinSessionCompletion()
{
    QCC_DbgTrace(("WaitForJoinSessionCompletion"));

    unsigned int secondsCount = 0;

    while (true) {
        QCC_DbgTrace(("WaitForJoinSessionCompletion1"));
        if (0 == (secondsCount++ % 10)) {
            QCC_DbgTrace(("Waited %u seconds for JoinSession completion.", secondsCount / 10));
        }

        if (secondsCount > 20) {
            break;
        }
        usleep(1000 * 1000);

        if (m_ContentSourceClientList.size() > 0) {
            QCC_DbgTrace(("WaitForJoinSessionCompletion: Has source"));
            return ER_OK;
        }
    }

    QCC_DbgTrace(("WaitForJoinSessionCompletion: No session"));
    return ER_ALLJOYN_JOINSESSION_REPLY_CONNECT_FAILED;
}

QStatus SampleContentServiceClient::RunExamples()
{
    QCC_DbgTrace(("SampleContentServiceClient::RunExamples"));

    QStatus status =  WaitForJoinSessionCompletion();
    if (status != ER_OK) {
        QCC_DbgTrace(("RunExamples - no source found"));
        return status;
    }

    ContentSourceClient* contentSourceClient = m_ContentSourceClientList.front();

    if (!contentSourceClient->GetHasSession()) {
        QStatus status = contentSourceClient->JoinSession();
        if (status != ER_OK) {
            QCC_LogError(status, ("RunExamples: Failed to join session"));
            return ER_OPEN_FAILED;
        }
    }

    SampleContentServiceCalls sampleContentServiceCalls(contentSourceClient->GetContentProxyBusObject());

    sampleContentServiceCalls.TestAllCalls();

    return ER_OK;
}

void SampleContentServiceClient::Announce(uint16_t version, uint16_t sessionPort, const char* busName, const ObjectDescriptions& objectDescs, const AboutData& aboutData)
{
    QCC_DbgTrace(("Announce: busName=%s, version=%d, sessionPort=%d, myUniqueName=%s", busName, version, sessionPort, m_BusAttachment->GetUniqueName().c_str()));

    for (auto i = aboutData.begin(); i != aboutData.end(); ++i) {
        QCC_DbgPrintf(("aboutData: key=%s", i->first.c_str()));
    }

    if (m_BusAttachment->GetUniqueName().compare(busName) == 0) {
        QCC_DbgPrintf(("This is an announcement from our bus so ignoring."));
        return;
    }

    QCC_DbgTrace(("Announce: Adding BusName=%s", busName));
    qcc::String contentSourcePrefix = s_CONTENT_OBJECT_PATH;

    ajn::services::AboutClient::ObjectDescriptions::const_iterator it;

    for (it = objectDescs.begin(); it != objectDescs.end(); ++it) {
        qcc::String key = it->first;
        std::vector<qcc::String> interfaces = it->second;

        QCC_DbgTrace(("Announce: key=%s", key.c_str()));
        for (uint i = 0; i < interfaces.size(); i++) {
            QCC_DbgTrace(("Announce: interface=%s", interfaces[i].c_str()));
        }

        if (key.compare(0, contentSourcePrefix.size(), contentSourcePrefix) == 0) {
            QCC_DbgTrace(("Found a new content source on %s, key was: %s", busName, key.c_str()));
            //m_BusAttachment->EnableConcurrentCallbacks();

            ContentSourceClient* contentSourceClient = new ContentSourceClient(m_BusAttachment, busName, 0, sessionPort);

            m_ContentSourceClientList.push_back(contentSourceClient);
        }
    }
}

} //mdf
