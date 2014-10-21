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

#include "GetContentSourceInfoCommand.h"
#include "CommandUtil.h"
#include <vector>
#include <MsgArg.h>

namespace mdf {

using namespace std;
using namespace qcc;

GetContentSourceInfoCommand::GetContentSourceInfoCommand(ContentSourceTracker* contentSourceTracker) :
    m_CSTracker(contentSourceTracker)
{

}

bool GetContentSourceInfoCommand::Execute()
{
    bool showMenu = true;
    ContentSource* cs = m_CSTracker->GetSelectedContentSource();
    if (!cs) {
        cout << "You must select a content source to call GetContentSourceInfo!\n" << endl;
        return true;
    }

    const MsgArg contentArg = cs->GetContentSourceInfo();
    size_t count;
    MsgArg* info;
    QStatus err = contentArg.Get("a{sv}", &count, &info);
    if (err == ER_OK) {
        for (int i = 0; i < count; i++) {
            char* name;
            MsgArg* valueArg;
            err = info[i].Get("{sv}", &name, &valueArg);

            if (err == ER_OK) {
                if (name != NULL) {
                    String nameString(name);
                    if (nameString.compare("mimeTypes") == 0) {
                        size_t numberOfMimetypes;
                        MsgArg* mimes;
                        err = valueArg->Get("as", &numberOfMimetypes, &mimes);
                        if (err == ER_OK) {
                            cout << "\n******************** Supported MIMETypes ********************" << endl;
                            for (int j = 0; j < numberOfMimetypes; j++) {
                                cout << mimes[j].v_string.str << endl;
                            }
                            cout << "******************** END Supported MIMETypes ****************" << endl;
                        } else {
                            cout << "\nError extracting mime types" << endl;
                        }
                    } else if (nameString.compare("transports") == 0) {
                        size_t numberOfTransports;
                        MsgArg* transports;
                        err = valueArg->Get("aq", &numberOfTransports, &transports);
                        if (err == ER_OK) {
                            cout << "\n******************** Supported Transports ********************" << endl;
                            for (int j = 0; j < numberOfTransports; j++) {
                                cout << transports[j].v_uint16 << endl;
                            }
                            cout << "******************** END Supported Transports ****************" << endl;
                        } else {
                            cout << "\nError extracting transports" << endl;
                        }
                    } else {
                        cout << "Unknown value for " << nameString << endl;
                    }
                } else {
                    cout << "The name of the property at " << i << " was null." << endl;
                }
            } else {
                cout << "There was a problem getting the content info for " << i << endl;
            }
        }
    } else {
        cout << "\nError opening the content info object: " << err << endl;
    }

    return showMenu;
}

const char* GetContentSourceInfoCommand::GetMenuText()
{
    return "GetContentSourceInfo";
}
}

