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


#include "SelectContentSource.h"
#include <map>
#include <string>

namespace mdf {

using namespace std;
using namespace qcc;

SelectContentSource::SelectContentSource(ContentSourceTracker* contentSourceTracker) : m_CSTracker(contentSourceTracker) {
}

bool SelectContentSource::Execute()
{
    bool showMenu = true;
    const ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    String selectedID = (cs ? cs->GetID() : "");

    cout << "\n**************** Content Sources ******************" << endl;
    map<String, ContentSource*> contentSources = m_CSTracker->GetContentSources();
    for (std::map<String, ContentSource*>::const_iterator iter = contentSources.begin(); iter != contentSources.end(); ++iter) {
        String id = (*iter).first;
        const ContentSource* contentSource = (*iter).second;
        if (selectedID.compare(id) == 0) {
            cout << "*(" << id << "): " << contentSource->GetDisplayName() << endl;
        } else {
            cout << "(" << id << "):  " << contentSource->GetDisplayName() << endl;
        }
    }
    cout << "***************************************************\n" << endl;
    cout << "Please enter the ID of your selected ContentSource: " << endl;

    string input;
    cin >> input;
    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        cout << "Invalid ID entered!" << endl;
    } else {
        String valueToFind = input.c_str();
        bool contentSourceSelected = m_CSTracker->SetSelectedContentSource(valueToFind);
        if (contentSourceSelected) {
            cout << "ContentSource Selected!\n" << endl;
        } else {
            cout << "ContentSource with the ID: " << valueToFind << " was not found!\n" << endl;
        }
    }

    return showMenu;
}

const char* SelectContentSource::GetMenuText()
{
    return "Select ContentSource";
}
}

