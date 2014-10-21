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

#include "DisplayDevicesCommand.h"
#include <map>

namespace mdf {

using namespace std;
using namespace qcc;

DisplayDevicesCommand::DisplayDevicesCommand(ContentSourceTracker* contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{

}

bool DisplayDevicesCommand::Execute()
{
    bool showMenu = true;
    ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    String selectedID = (cs ? cs->GetID() : "");

    cout << "\n**************** Content Sources ******************" << endl;
    map<String, ContentSource*> contentSources = m_CSTracker->GetContentSources();
    for (std::map<String, ContentSource*>::iterator iter = contentSources.begin(); iter != contentSources.end(); ++iter) {
        String id = (*iter).first;
        const ContentSource* contentSource = (*iter).second;
        if (selectedID.compare(id) == 0) {
            cout << "*(" << id << ") " << contentSource->GetDisplayName() << endl;
        } else {
            cout << "(" << id << ") " << contentSource->GetDisplayName() << endl;
        }
    }
    cout << "***************************************************\n" << endl;
    return showMenu;
}

const char* DisplayDevicesCommand::GetMenuText()
{
    return "Display Devices";
}
}

