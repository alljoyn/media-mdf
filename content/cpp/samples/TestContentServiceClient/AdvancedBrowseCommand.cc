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

#include "AdvancedBrowseCommand.h"
#include "CommandUtil.h"

#include <qcc/Debug.h>
#include <qcc/Log.h>
#include <vector>
#include <string>

#define QCC_MODULE "AdvancedBrowseCommand"

namespace mdf {
using namespace std;
using namespace qcc;

AdvancedBrowseCommand::AdvancedBrowseCommand(ContentSourceTracker*contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{
}

bool AdvancedBrowseCommand::Execute()
{

    const ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    if (!cs) {
        cout << "You must select a content source to browse it!\n" << endl;
        return true;
    }

    //Let's do the initial browse for the user
    MsgArg* arg = new MsgArg();

    std::vector<SortBy> listTosortBy;
    SortBy sortA;
    sortA.propertyToSortBy = "id";
    sortA.order = 0;
    listTosortBy.push_back(sortA);

    const QueryResponse* resp = cs->Browse("0", *arg, 0, 100, listTosortBy);
    if (resp != NULL) {
        const vector<Node*> nodes = resp->GetItems();
        cout << "\nBrowse returned " << nodes.size() << " items.\n" << endl;
        CommandUtil::PrintNodes(nodes);
        bool displayMenu = true;
        while (displayMenu) {

            cout << "Please enter the itemID of the node you want to browse into:" << endl;
            string itemId;
            cin >> itemId;
            if (!CommandUtil::CINValid()) {
                cout << "Invalid ID entered!" << endl;
                break;
            }

            cout << "Please enter the offset:" << endl;
            uint16_t offset;
            cin >> offset;
            if (!CommandUtil::CINValid()) {
                cout << "Invalid offset entered!" << endl;
                break;
            }

            cout << "Please enter the the maximum number of items you want returned:" << endl;
            uint16_t limit;
            cin >> limit;
            if (!CommandUtil::CINValid()) {
                cout << "Invalid limit entered!" << endl;
                break;
            }

            String id(itemId.c_str());
            resp = cs->Browse(id, *arg, offset, limit, listTosortBy);
            if (resp != NULL) {
                cout << "\nBrowse returned " << nodes.size() << " items.\n" << endl;
                CommandUtil::PrintNodes(nodes);
            } else {
                cout << "\nBrowse returned NULL!\n" << endl;
            }
            displayMenu = false;
        }

        delete resp;
        cout << "\n\n" << endl;
    } else {
        cout << "Browse returned NULL!\n" << endl;
    }

    if (arg) {
        delete arg;
    }

    return true;
}

const char* AdvancedBrowseCommand::GetMenuText()
{
    return "Advanced Browse";
}
}

