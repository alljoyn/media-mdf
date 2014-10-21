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

#ifndef CONTENTSERVICESERVER_H_
#define CONTENTSERVICESERVER_H_

#include <stdint.h>
#include <map>
#include <vector>

#include <alljoyn/BusAttachment.h>


using namespace ajn;

namespace mdf {

class ContentServiceSessionHandler;
class ContentAnnounceHandlerServer;
class ContentSourceListener;


class ContentServiceServer {

  public:

    /*
     * @function GetInstance
     * @description singleton constructor method. Only one instance of this should ever be running per machine. This returns the current
     * instance or creates a new one if none exists
     * @return a pointer to the ContentServiceServer instance
     */
    static ContentServiceServer* GetInstance();

    /*
     * @function ~ContentService
     * @description destructor
     */
    ~ContentServiceServer();

    /*
     * @function Init
     * @description this should be called only once on the instance of the server. Calling it more than once will result in an error being thrown
     * A set of parameters are passed to the server containing all information that the server needs
     * @param contentServerParams the parameters the server needs to run
     */
    QStatus Init(ajn::BusAttachment* busAttachment);

    /*
     * @function Shutdown
     * @description Cleanup and get ready for shutdown.
     */
    void Shutdown();

    /*
     * @function GetServerVersion
     * @description Gets the server version
     * @return version
     */
    uint16_t GetServerVersion() const;

    /*
     * @function GetContentServiceSessionHandler
     * @description returns a pointer to the ContentServiceSessionHandler
     * @return ContentServiceSessionHandler*
     */
    ContentServiceSessionHandler* GetContentServiceSessionHandler();

  private:

    //SessionListener
    void SessionLost(SessionId sessionId, ajn::SessionListener::SessionLostReason reason);


  private:

    ContentServiceServer();  // Private so that it can not be called because this is a singleton
    ContentServiceServer(ContentServiceServer const&); // copy constructor is private not be called because this is a singleton
    ContentServiceServer& operator=(ContentServiceServer const&); // assignment operator is private not be called because this is a singleton

    static ContentServiceServer* s_Instance;

    std::vector<const ContentSourceListener*> m_ContentSourceListeners;     //vector of references to contentsourcelisteners

    ContentServiceSessionHandler* m_ContentServerSessionHandler;
    ContentAnnounceHandlerServer* m_ContentAnnounceHandlerServer;

};

} //namespace ajn


#endif /* CONTENTSERVICESERVER_H_ */
