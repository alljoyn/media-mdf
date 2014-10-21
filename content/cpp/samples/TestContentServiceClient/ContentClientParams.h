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

#ifndef CONTENTCLIENTPARAMS_H_
#define CONTENTCLIENTPARAMS_H_

#include <alljoyn/BusAttachment.h>

using namespace ajn;

namespace mdf {

class ContentClientParams {

  public:

    /*
     * @function ContentClientParams
     * @description constructor.
     */
    ContentClientParams();

    /*
     * @function ~ContentClientParams
     * @description default destructor
     */
    virtual ~ContentClientParams();

    /*
     * @function GetBusAttachment
     * @description gets the bus attachment
     * @return const BusAttachment& the bus attachment associated with the object
     * if one has been added. Note - If none is supplied then the class will create
     * one for you. You should only need to create your own in special circumstances
     */
    BusAttachment* GetBusAttachment();

    /*
     * @function GetSessionOpts
     * @description returns the session opts.
     * @retVal SessionOpts if one was set by the user
     */
    const SessionOpts* GetSessionOpts();

    /*
     * @function Init
     * @description This should not be called by the user of this API. It is called by the content client. Calling this prematurely will cause problems
     * @return ER_OK if ok else a range of errors
     */
    QStatus Init();

  private:

    /*
     * @function CreateBusAttachment
     * @description Creates a bus attachment
     * @return ER_OK if the bus attachment has been created successfully, otherwise a range of errors
     */
    QStatus CreateBusAttachment();

    /*
     * @function CreateSessionOpts
     * @description creates the session opts
     * @return ER_OK if the SessopnOpts have been created successfully, otherwise a range of errors
     */
    QStatus CreateSessionOpts();


  private:

    BusAttachment* m_Bus;
    SessionOpts* m_sessionOpts;
};

} /* mdf */

#endif /* ContentClientParams_H_ */
