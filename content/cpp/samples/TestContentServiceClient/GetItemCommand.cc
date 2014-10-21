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

#include "GetItemCommand.h"
#include "CommandUtil.h"

namespace mdf {

using namespace std;
using namespace qcc;

GetItemCommand::GetItemCommand(ContentSourceTracker* contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{

}

bool GetItemCommand::Execute()
{
    bool showMenu = true;
    ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    if (!cs) {
        cout << "You must select a content source to call GetItem!\n" << endl;
        return true;
    }

    bool displayMenu = true;
    while (displayMenu) {
        cout << "Please enter the itemID of the node you want to fetch or 0 to exit:" << endl;
        string input;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid ID entered!" << endl;
        } else {
            if (input.compare("0") == 0) {
                displayMenu = false;
            } else {
                String id(input.c_str());
                Node* node = cs->GetItem(id);
                if (node != NULL) {
                    cout << "\n" << endl;
                    CommandUtil::PrintNode(node);
                    cout << "\n" << endl;
                    delete node;
                } else {
                    cout << "\nGetItem returned NULL!\n" << endl;
                }
            }
        }
    }

    return showMenu;
}

const char* GetItemCommand::GetMenuText()
{
    return "GetItem";
}
}

