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

#include "ContentSourceTracker.h"
#include <iostream>

namespace mdf {


ContentSourceTracker::~ContentSourceTracker()
{
    m_CSLock.Lock(MUTEX_CONTEXT);
    for (std::map<String, ContentSource*>::const_iterator iter = m_ContentSources.begin(); iter != m_ContentSources.end(); ++iter) {
        ContentSource* cs = (*iter).second;
        if (cs) {
            delete cs;
        }
    }
    m_ContentSources.clear();
    m_CSLock.Unlock(MUTEX_CONTEXT);
}

void ContentSourceTracker::ContentSourceAdded(ContentSource& contentSource) {
    //Needed to make the m_ContentSources mutable due to the const at the end.. is that ok?
    m_CSLock.Lock(MUTEX_CONTEXT);
    m_ContentSources.insert(std::pair<String, ContentSource*>(contentSource.GetID(), &contentSource));
    m_CSLock.Unlock(MUTEX_CONTEXT);

    if (!m_ContentSource) {
        //Set the first content source found as selected by default
        m_ContentSource = &contentSource;
    }
}

void ContentSourceTracker::ContentSourceLost(ContentSource& contentSource) {
    cout << "\nContentSource lost: " << contentSource.GetDisplayName() << endl;
    String contentID = contentSource.GetID();
    bool contentSourceFound = false;
    ContentSource* lostContentSource;
    m_CSLock.Lock(MUTEX_CONTEXT);
    for (std::map<String, ContentSource*>::const_iterator iter = m_ContentSources.find(contentID); iter != m_ContentSources.end(); ++iter) {
        contentSourceFound = true;
        lostContentSource = (*iter).second;
        if (contentID.compare(lostContentSource->GetID()) == 0) {
            m_ContentSource = NULL;
        }
        break;
    }

    if (contentSourceFound) {
        m_CSLock.Lock(MUTEX_CONTEXT);
        m_ContentSources.erase(contentID);
        m_CSLock.Unlock(MUTEX_CONTEXT);
        delete lostContentSource;
    }

    if (!m_ContentSource) {
        //Select the next content source as selected if we lost the selected ONE...
        for (std::map<String, ContentSource*>::const_iterator iter = m_ContentSources.begin(); iter != m_ContentSources.end(); ++iter) {
            m_ContentSource = (*iter).second;
            break;
        }
    }
    m_CSLock.Unlock(MUTEX_CONTEXT);
}

map<String, ContentSource*> ContentSourceTracker::GetContentSources() const {
    return m_ContentSources;
}

bool ContentSourceTracker::SetSelectedContentSource(String contentSourceID) {
    bool contentSourceFound = false;
    for (std::map<String, ContentSource*>::const_iterator iter = m_ContentSources.find(contentSourceID); iter != m_ContentSources.end(); ++iter) {
        ContentSource* selectedCS = (*iter).second;
        m_ContentSource = selectedCS;
        contentSourceFound = true;
        break;
    }
    return contentSourceFound;
}

ContentSource* ContentSourceTracker::GetSelectedContentSource() const {
    return m_ContentSource;
}

}
