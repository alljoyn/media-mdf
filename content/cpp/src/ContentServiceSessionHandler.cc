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

#include "ContentServiceSessionHandler.h"
#include <LogModule.h>
#include <sstream>

using namespace ajn;
//using namespace services;

namespace mdf {

/**
 * Constructor for MySessionHandler
 */
ContentServiceSessionHandler::ContentServiceSessionHandler() : m_SessionId(0), m_HasJoined(false), TAG("MySessionHandler") {
}

/**
 * Destructor for MySessionHandler
 */
ContentServiceSessionHandler::~ContentServiceSessionHandler() {

    m_SessionId = 0;
    m_HasJoined = false;
}

/**
 * SessionLost
 * @param sessionId
 */
void ContentServiceSessionHandler::SessionLost(ajn::SessionId sessionId) {
    std::stringstream sessionIdStr;
    sessionIdStr << sessionId;
    QCC_DbgHLPrintf(("Session lost for sessionId: %s", sessionIdStr.str().c_str()));

    m_SessionId = 0;
}

/**
 * JoinSessionCB
 * @param status
 * @param id
 * @param opts
 * @param context
 */
void ContentServiceSessionHandler::JoinSessionCB(QStatus status, ajn::SessionId id, const ajn::SessionOpts& opts, void* context)
{
    if (status != ER_OK) {
        QCC_LogError(status, ("Joining session failed."));
        return;
    }
    std::stringstream sessionIdStr;
    sessionIdStr << id;
    QCC_DbgHLPrintf(("Joining session succeeded. SessionId:  %s", sessionIdStr.str().c_str()));

    m_SessionId = id;
    m_HasJoined = true;
}

bool ContentServiceSessionHandler::HasJoined() {
    return m_HasJoined;
}

} //end ajn
