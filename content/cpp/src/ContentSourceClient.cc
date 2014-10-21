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

#include "ContentSourceClient.h"
#include "LogModule.h"

namespace mdf {

ContentSourceClient::ContentSourceClient(BusAttachment* busAttachment, String busName, SessionId sessionId, SessionPort sessionPort) : m_BusAttachment(busAttachment), m_BusName(busName),  m_SessionId(sessionId), m_SessionPort(sessionPort), m_HasSession(false), m_ContentProxyBusObject(NULL), m_SessionOpts(NULL)
{
    QCC_DbgTrace(("ContentSourceClient: created"));

    m_ContentProxyBusObject = NULL;
    m_SessionOpts = new SessionOpts(SessionOpts::TRAFFIC_MESSAGES, false, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);
    m_ContentProxyBusObject = new ContentProxyBusObject(m_BusAttachment, m_BusName, m_SessionId);
}

ContentSourceClient::~ContentSourceClient()
{
    QCC_DbgTrace(("ContentSourceClient: Destructor"));

    delete m_SessionOpts;
    delete m_ContentProxyBusObject;
}

uint16_t ContentSourceClient::GetVersion()
{
    return m_Version;
}

void ContentSourceClient::SetVersion(uint16_t version)
{
    m_Version = version;
}

bool ContentSourceClient::GetHasSession()
{
    QCC_DbgTrace(("ContentSourceClient::GetHasSession"));
    return m_HasSession;
}

SessionListener* ContentSourceClient::GetSessionListener()
{
    QCC_DbgTrace(("ContentSourceClient::GetSessionListener=%ld", this));
    return this;
}

void ContentSourceClient::SetHasSession(bool hasSession)
{
    m_HasSession = hasSession;
}

//SessionListener
void ContentSourceClient::SessionLost(SessionId sessionId, SessionLostReason reason)
{
    QCC_DbgTrace(("ContentSourceClient::SessionLost"));
    SetHasSession(false);
}

QStatus ContentSourceClient::JoinSession()
{
    QCC_DbgTrace(("ContentSourceClient::JoinSession"));
    QStatus status = m_BusAttachment->JoinSession(m_BusName.c_str(), m_SessionPort, GetSessionListener(), m_SessionId, *m_SessionOpts);
    if (status != ER_OK) {
        QCC_LogError(status, ("ContentSourceClient: Failed to join session"));
        return status;
    }

    QCC_DbgTrace(("ContentSourceClient::JoinSession2"));
    SetHasSession(true);

    return ER_OK;
}

ContentProxyBusObject* ContentSourceClient::GetContentProxyBusObject()
{
    return m_ContentProxyBusObject;
}

} //mdf