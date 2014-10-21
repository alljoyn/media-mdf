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

#ifndef CONTENTSERVERPARAMS_H_
#define CONTENTSERVERPARAMS_H_

#include "MdfContentSource.h"
#include <alljoyn/BusAttachment.h>

#include "alljoyn/about/AboutPropertyStoreImpl.h"

using namespace ajn::services;

namespace mdf {

class ContentServerBusObject;
class ContentBusListener;
class ContentSessionPortListener;

class SampleContentService {
  public:

    /*
     * @function SampleContentService
     * @description constructor. Note that this class must have at least one
     * content source
     * @param contentSource a default content source
     */
    SampleContentService();

    /*
     * @function ~SampleContentService
     * @description default destructor
     */
    virtual ~SampleContentService();

    /*
     * @function Init
     * @description This should not be called by the user of this API. It is called by the content server. Calling this prematurely will cause problems
     * @return ER_OK if ok else a range of errors
     */
    QStatus Init();

  private:

    /*
     * @function CreateAboutPropertyStore
     * @description creates the property store
     * @return AboutPropertyStoreImpl if the property store has been created successfully, otherwise NULL
     */
    AboutPropertyStoreImpl* CreateAboutPropertyStore();


    QStatus InitAboutService(BusAttachment* busAttachment, SessionPort sessionPort);

    QStatus InitContentModel();

    QStatus InitContentComms();

  private:

    SessionPort m_SessionPort;
    BusAttachment* m_BusAttachment;
    SessionOpts* m_sessionOpts;
    AboutPropertyStoreImpl* m_AboutPropertyStore;

    MdfContentSource* m_MdfContentSource;

    ContentServerBusObject* m_ContentServerBusObject;
    AboutServiceApi* m_AboutService;

    ContentBusListener* m_ContentBusListener;
    ContentSessionPortListener* m_ContentSessionPortListener;
};

} /* mdf */

#endif /* CONTENTSERVERPARAMS_H_ */
