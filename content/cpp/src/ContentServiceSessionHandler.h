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

#ifndef CONTENTSERVICESESSIONHANDLER_H_
#define CONTENTSERVICESESSIONHANDLER_H_

#include <alljoyn/SessionListener.h>
#include <alljoyn/BusAttachment.h>

namespace mdf {


class ContentServiceSessionHandler : public ajn::BusAttachment::JoinSessionAsyncCB, public ajn::SessionListener {
  public:

    /**
     * Constructor for MySessionHandler
     */
    ContentServiceSessionHandler();

    /**
     * Destructor for MySessionHandler
     */
    virtual ~ContentServiceSessionHandler();

    /**
     * SessionLost
     * @param sessionId
     */
    virtual void SessionLost(ajn::SessionId sessionId);

    /**
     * JoinSessionCB
     * @param status
     * @param id
     * @param opts
     * @param context
     */
    virtual void JoinSessionCB(QStatus status, ajn::SessionId id, const ajn::SessionOpts& opts, void* context);

    virtual bool HasJoined();

  private:

    /**
     * SessionId for this Device
     */
    ajn::SessionId m_SessionId;

    bool m_HasJoined;

    qcc::String TAG;

};

} //mdf



#endif /* CONTENTSERVICESESSIONHANDLER_H_ */
