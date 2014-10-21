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

#ifndef CONTENTBUSLISTENER_H_
#define CONTENTBUSLISTENER_H_

#include <alljoyn/BusListener.h>

namespace mdf {

class ContentBusListener : public ajn::BusListener {
  public:

    /**
     * Constructor of ContentBusListener
     * @param bus - used to set a session Listener
     * @param daemonDisconnectCB - used to set a callback for when the daemon is disconnected
     */
    ContentBusListener();

    /**
     * Destructor of ContentBusListener
     */
    virtual ~ContentBusListener();

    /**
     * Function when Bus has been disconnected
     */
    void BusDisconnected();



};

} /* namespace mdf */

#endif /* CONTENTBUSLISTENER_H_ */
