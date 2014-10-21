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

#include "SearchCommand.h"
#include "MsgArg.h"
#include "CommandUtil.h"

#include <qcc/Debug.h>
#include <qcc/Log.h>
#include <vector>
#include <string>
#include <sstream>

#define QCC_MODULE "SearchCommand"

namespace mdf {
using namespace std;
using namespace qcc;

SearchCommand::SearchCommand(ContentSourceTracker*contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{
}

bool SearchCommand::Execute()
{
    const ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    if (!cs) {
        cout << "You must select a content source to Search it!\n" << endl;
        return true;
    }

    cout << "Please enter your search string: " << endl;
    string input;
    for (; std::getline(cin, input);) {
        if (input.length() > 0) {
            break;
        }
    }

    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        cout << "Invalid search string entered!" << endl;
    } else {
        //Search arguments!!
        String searchString(input.c_str());
        uint16_t searchMode = 1;     // 0 = startswith, 1 = contains
        uint16_t searchType = 0;     // 0 = All, 1 = Audio, 2 = Images, 3 = Video
        MsgArg* playerCapabilities = new MsgArg();
        uint64_t offset = 0;
        uint32_t limit = 100;
        std::vector<SortBy> listTosortBy;
        SortBy sortA;
        sortA.propertyToSortBy = "id";
        sortA.order = 0;     //ASC
        listTosortBy.push_back(sortA);
        //END

        const QueryResponse* resp = cs->Search(searchString, searchMode, searchType, *playerCapabilities, offset,
                                               limit, listTosortBy);
        if (resp != NULL) {
            const vector<Node*> nodes = resp->GetItems();
            cout << "\nSearch returned " << nodes.size() << " items.\n" << endl;
            CommandUtil::PrintNodes(nodes);
            bool displayMenu = true;
            while (displayMenu) {
                cout << "Please enter the searchString or 0 to exit:" << endl;
                for (; std::getline(cin, input);) {
                    if (input.length() > 0) {
                        break;
                    }
                }

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    cout << "Invalid search string entered!" << endl;
                } else {
                    if (input.compare("0") == 0) {
                        displayMenu = false;
                    } else {
                        String query(input.c_str());
                        resp = cs->Search(query, searchMode, searchType, *playerCapabilities, offset, limit,
                                          listTosortBy);
                        if (resp != NULL) {
                            cout << "\nSearch returned " << nodes.size() << " items.\n" << endl;
                            CommandUtil::PrintNodes(nodes);
                        } else {
                            cout << "\nSearch returned NULL!\n" << endl;
                        }
                    }
                }
            }
            delete playerCapabilities;
            delete resp;
            cout << "\n\n" << endl;
        } else {
            cout << "Search returned NULL!\n" << endl;
        }
    }

    return true;
}

const char* SearchCommand::GetMenuText()
{
    return "Search";
}
}

