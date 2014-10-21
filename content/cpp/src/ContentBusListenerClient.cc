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

#include "ContentBusListenerClient.h"
#include "ContentConstants.h"
#include <LogModule.h>

namespace mdf {

using namespace ajn;
using namespace qcc;

ContentBusListenerClient::ContentBusListenerClient(BusAttachment* busAttachment, SessionListener* sessionListener, SessionPort sessionPort, SessionId sessionId)
{
    QCC_DbgTrace(("Init - ContentBusListenerClient sessionId=%d,sessionPort=%d", sessionId, sessionPort));
    m_SessionId = sessionId;
    m_FoundAdvertisedName = false;
    m_BusAttachment = busAttachment;
    m_SessionListner = sessionListener;
}

ContentBusListenerClient::~ContentBusListenerClient()
{
}

bool ContentBusListenerClient::FoundAdvertisedName()
{
    return m_FoundAdvertisedName;
}

void ContentBusListenerClient::FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix)
{
    QCC_DbgTrace(("FoundAdvertisedName name=%s", name));
    if (s_CONTENT_INTERFACE == name) {
        /* We found a remote bus that is advertising basic service's well-known name so connect to it. */
        /* Since we are in a callback we must enable concurrent callbacks before calling a synchronous method. */
        m_BusAttachment->EnableConcurrentCallbacks();

        SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, false, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);

        QStatus status = m_BusAttachment->JoinSession(name, m_SessionPort, m_SessionListner, m_SessionId, opts);
        if (status != ER_OK) {
            QCC_LogError(status, ("m_BusAttachment->JoinSession failed"));
            return;
        }

    }
    m_FoundAdvertisedName = true;
}

void ContentBusListenerClient::NameOwnerChanged(const char* busName, const char* previousOwner, const char* newOwner)
{
    QCC_DbgTrace(("NameOwnerChanged busName=%s, previous=%s, newOwner=%s", busName, previousOwner, newOwner));
    if (newOwner && (s_CONTENT_INTERFACE == busName)) {
        QCC_DbgTrace(("NameOwnerChanged: name='%s', oldOwner='%s', newOwner='%s'.\n",
                      busName,
                      previousOwner ? previousOwner : "<none>",
                      newOwner ? newOwner : "<none>"));
    }
}

} /* namespace mdf */
