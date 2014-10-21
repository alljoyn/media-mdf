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

#ifndef ContentSessionListener_H_
#define ContentSessionListener_H_

//#include <vector>

#include <alljoyn/SessionListener.h>

namespace mdf {

class ContentSessionListener : public ajn::SessionListener {

  public:

    /**
     * Constructor of ContentSessionListener
     * @param bus - used to set a session Listener
     * @param daemonDisconnectCB - used to set a callback for when the daemon is disconnected
     */
    ContentSessionListener();

    /**
     * Destructor of ContentSessionListener
     */
    ~ContentSessionListener();

    /**
     * Callback for when Session is lost
     * @param sessionId
     * @param reason for session lost
     */
    void SessionLost(ajn::SessionId sessionId, SessionLostReason reason);


  private:
    /**
     * The sessionIds for the port
     */
    //std::vector<ajn::SessionId> m_SessionIds;

};

} /* namespace mdf */

#endif /* ContentSessionListener_H_ */
