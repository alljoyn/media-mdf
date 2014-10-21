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

#include "GetSortableFieldsCommand.h"
#include "CommandUtil.h"
#include <vector>

namespace mdf {

using namespace std;
using namespace qcc;

GetSortableFieldsCommand::GetSortableFieldsCommand(ContentSourceTracker* contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{

}

bool GetSortableFieldsCommand::Execute()
{
    bool showMenu = true;
    ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    if (!cs) {
        cout << "You must select a content source to call GetSortableFields!\n" << endl;
        return true;
    }

    vector<String> sortableFields = cs->GetSortableFields();
    cout << "\n**************** Sortable Fields ******************" << endl;
    for (int i = 0; i < sortableFields.size(); i++) {
        cout << sortableFields.at(i) << endl;
    }
    cout << "\n**************** END Sortable Fields **************\n" << endl;
    return showMenu;
}

const char* GetSortableFieldsCommand::GetMenuText()
{
    return "GetSortableFields";
}
}

