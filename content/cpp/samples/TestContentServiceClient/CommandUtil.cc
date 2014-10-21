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

#include "CommandUtil.h"
#include <qcc/Debug.h>
#include <qcc/Log.h>
#include <limits>

#define QCC_MODULE "CommandUtil"

namespace mdf {

void CommandUtil::PrintNodes(const vector<Node*> nodes)
{
    for (int i = 0; i < nodes.size(); i++) {
        CommandUtil::PrintNode(nodes.at(i));
    }
    cout << "\n\n" << endl;
}

void CommandUtil::PrintNode(Node* node) {
    if (!node) {
        cout << "\n\nInval node submitted!\n\n" << endl;
    }

    String* props = GetProperties(node);
    cout << "[ItemId: " << node->GetItemID() <<
    ", title: " << node->GetTitle() <<
    ", type:" << node->GetTypeAsString() <<
    ", \nproperties {\n" << props->c_str() <<
    "\n}" <<
    "]" << endl;
    delete props;
}

String* CommandUtil::GetProperties(Node* node)
{
    String* resp = new String();
    size_t count;
    MsgArg* items;
    QStatus err = node->GetProperties().Get("a{sv}", &count, &items);
    if (err == ER_OK) {
        for (int i = 0; i < count; i++) {
            char* name;
            MsgArg* valueArg;
            err = items[i].Get("{sv}", &name, &valueArg);

            if (err == ER_OK) {
                if (name != NULL) {
                    String nameString(name);
                    String value;
                    if (valueArg != NULL) {
                        //TODO This may not be a string!
                        value = valueArg->v_string.str;
                    } else {
                        value = "";
                    }
                    if (i > 0) {
                        resp->append("\n");
                    }
                    resp->append(nameString);
                    resp->append("=");
                    resp->append(value);
                } else {
                    QCC_LogError(err, ("The name of the item at %d was null", i));
                }
            } else {
                QCC_LogError(err, ("There was a problem getting the item %i in the properties object", i));
            }
        }
    } else {
        QCC_LogError(err, ("There was a problem opening the properties object"));
    }

    return resp;
}

bool CommandUtil::CINValid() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}
}

