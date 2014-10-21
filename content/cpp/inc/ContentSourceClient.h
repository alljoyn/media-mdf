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

#ifndef __mdfContentService__ContentSourceClient__
#define __mdfContentService__ContentSourceClient__

#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusListener.h>
#include "ContentProxyBusObject.h"

using namespace ajn;
using namespace qcc;

namespace mdf {

class ContentSourceClient : public SessionListener {
  public:

    ContentSourceClient(BusAttachment* busAttachment, String busName, SessionId sessionId, SessionPort sessionPort);
    virtual ~ContentSourceClient();

    uint16_t GetVersion();
    void SetVersion(uint16_t version);

    bool GetHasSession();

    ContentProxyBusObject* GetContentProxyBusObject();

    QStatus JoinSession();

  private:
    //SessionListener
    void SessionLost(SessionId sessionId, SessionLostReason reason);
    void SetHasSession(bool hasSession);
    SessionListener* GetSessionListener();

  private:
    uint16_t m_Version;
    SessionPort m_SessionPort;
    String m_BusName;
    SessionId m_SessionId;
    BusAttachment* m_BusAttachment;
    SessionOpts* m_SessionOpts;
    bool m_HasSession;
    ContentProxyBusObject* m_ContentProxyBusObject;

};

} //mdf
#endif /* defined(__mdfContentService__ContentSourceClient__) */
