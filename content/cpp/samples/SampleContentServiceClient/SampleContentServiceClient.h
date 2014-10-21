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

#ifndef SampleContentServiceClient_H
#define SampleContentServiceClient_H

#include <alljoyn/BusListener.h>
#include <alljoyn/about/AnnounceHandler.h>
#include <alljoyn/BusAttachment.h>

#include <list>

namespace mdf {

class ContentBusListenerClient;
class ContentSessionListenerClient;
class ContentSourceClient;

class SampleContentServiceClient : public ajn::BusListener, public ajn::services::AnnounceHandler {

  public:

    SampleContentServiceClient();

    virtual ~SampleContentServiceClient();

    QStatus Init();

    QStatus RunExamples();

  public:

    /*
     * @function ContentSourceAdded
     * @description called when a new content source has been added
     * @param contentSource a reference to the content source that was added
     */
//    virtual void ContentSourceAdded(MdfContentSource& contentSource);

    /*
     * @function ContentSourceLost
     * @description called when a new content source has been removed
     * @param contentSource a reference to the content source that was removed
     */
    // virtual void ContentSourceLost(mdf::MdfContentSource& contentSource);

  private:
    ajn::BusAttachment* CreateBusAttachment();
//    QStatus CreateInterface(ajn::BusAttachment* busAttachment);
    QStatus WaitForJoinSessionCompletion();

    void Announce(uint16_t version, uint16_t port, const char* busName, const ObjectDescriptions& objectDescs, const AboutData& aboutData);

  private:

    ajn::SessionPort m_ClientSessionPort;
    ajn::BusAttachment* m_BusAttachment;
//    ajn::SessionOpts* m_sessionOpts;
    ajn::SessionId m_ClientSessionId;
//    ContentAnnounceHandlerClient* m_ContentAnnounceHandlerClient;

    ContentBusListenerClient* m_ContentBusListenerClient;
    ContentSessionListenerClient* m_ContentSessionListenerClient;
    std::list<ContentSourceClient*> m_ContentSourceClientList;
};

} //mdf

#endif /* SampleContentServiceClient_H */
