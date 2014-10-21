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

#include <iostream>
#include <map>
#include "Menu.h"
#include "Command.h"
#include "ExitCommand.h"
#include "DisplayDevicesCommand.h"
#include "SelectContentSource.h"
#include "BrowseCommand.h"
#include "AdvancedBrowseCommand.h"
#include "SearchCommand.h"
#include "GetItemCommand.h"
#include "GetSortableFieldsCommand.h"
#include "GetContentSourceInfoCommand.h"

namespace mdf {

using namespace std;

Menu::Menu(ContentSourceTracker* csTracker) : m_CSTracker(csTracker)
{
    int i = 1;
    DisplayDevicesCommand* displayDevices = new DisplayDevicesCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, displayDevices));

    SelectContentSource* selectContentSource = new SelectContentSource(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, selectContentSource));

    BrowseCommand* browseCommand = new BrowseCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, browseCommand));

    AdvancedBrowseCommand* advancedBrowseCommand = new AdvancedBrowseCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, advancedBrowseCommand));

    SearchCommand* searchCommand = new SearchCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, searchCommand));

    GetItemCommand* getItemCommand = new GetItemCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, getItemCommand));

    GetSortableFieldsCommand* getSortableFieldsCommand = new GetSortableFieldsCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, getSortableFieldsCommand));

    GetContentSourceInfoCommand* getContentSourceInfoCommand = new GetContentSourceInfoCommand(m_CSTracker);
    m_Commands.insert(std::pair<int, Command*>(i++, getContentSourceInfoCommand));

    ExitCommand* exitCommand = new ExitCommand();
    m_Commands.insert(std::pair<int, Command*>(i++, exitCommand));
}

Menu::~Menu()
{
    for (std::map<int, Command*>::iterator iter = m_Commands.begin(); iter != m_Commands.end(); ++iter) {
        delete (*iter).second;
    }
    m_Commands.clear();
}

bool Menu::DisplayMenu()
{
    bool showMenu = true;
    int myChoice = 0;

    const ContentSource* cs = m_CSTracker->GetSelectedContentSource();

    cout << "\n******************** Main Menu ********************" << std::endl;
    cout << "Selected ContentSource: " << (cs ?  cs->GetDisplayName() : "No ContentSource found.") << "\n" << std::endl;
    int maxMenuOption;
    for (std::map<int, Command*>::iterator iter = m_Commands.begin(); iter != m_Commands.end(); ++iter) {
        maxMenuOption = (*iter).first;
        cout << "(" << maxMenuOption << "): " << (*iter).second->GetMenuText() << endl;
    }

    cin >> myChoice;
    if (cin.fail()) {
        //Someone did not enter a number!
        myChoice = 999;
        cin.clear();
        cin.ignore();
    }

    if (myChoice > maxMenuOption) {
        cout << "ERROR! You have selected an invalid choice." << endl;
    } else {
        showMenu = GetCommand(myChoice)->Execute();
    }

    return showMenu;
}

Command* Menu::GetCommand(int key)
{
    Command* value;
    for (std::map<int, Command*>::iterator iter = m_Commands.find(key); iter != m_Commands.end(); ++iter) {
        value = (*iter).second;
        break;
    }
    return value;
}
}

