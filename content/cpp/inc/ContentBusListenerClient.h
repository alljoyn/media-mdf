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

#ifndef CONTENTBUSLISTENER_CLIENT_H_
#define CONTENTBUSLISTENER_CLIENT_H_

#include <alljoyn/BusListener.h>
#include <alljoyn/BusAttachment.h>

using namespace ajn;

namespace mdf {

class ContentBusListenerClient : public BusListener {
  public:

    /**
     * Constructor of ContentBusListener
     * @param bus - used to set a session Listener
     * @param daemonDisconnectCB - used to set a callback for when the daemon is disconnected
     */
    ContentBusListenerClient(BusAttachment* busAttachment, SessionListener* sessionListener, SessionPort sessionPort, SessionId sessionId);

    /**
     * Destructor of ContentBusListener
     */
    virtual ~ContentBusListenerClient();

    virtual void FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix);


    virtual void NameOwnerChanged(const char* busName, const char* previousOwner, const char* newOwner);

    bool FoundAdvertisedName();

  private:

    SessionId m_SessionId;
    SessionPort m_SessionPort;
    bool m_FoundAdvertisedName;
    BusAttachment* m_BusAttachment;
    SessionListener* m_SessionListner;
};

} /* namespace mdf */

#endif /* CONTENTBUSLISTENER_CLIENT_H_ */
