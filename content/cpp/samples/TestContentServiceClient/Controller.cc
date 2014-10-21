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
#include <signal.h>
#include "Menu.h"
#include "ContentSourceTracker.h"
#include <ContentServiceClient.h>
#include <ContentClientParams.h>
#include <SrpKeyXListener.h>
#include <qcc/Debug.h>
#include <qcc/Log.h>
#include <CommonSampleUtil.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
using namespace mdf;

#define QCC_MODULE "Controller"

Menu* menu;
ContentClientParams* contentClientParams;
SrpKeyXListener* srpKeyXListener = 0;
ContentSourceTracker* contentSourceTracker = 0;

void cleanup()
{
    if (menu) {
        delete menu;
    }

    if (contentSourceTracker) {
        ContentServiceClient::GetInstance()->UnRegisterContentSourceListener(*contentSourceTracker);
        delete contentSourceTracker;
    }

    ContentServiceClient::GetInstance()->Shutdown();

    if (contentClientParams) {
        delete contentClientParams;
    }
}

static void signal_callback_handler(int32_t signum)
{
    cleanup();
    exit(signum);
}

void initServices() {

    //Init ContentServiceClient
    ContentServiceClient* csClient = ContentServiceClient::GetInstance();

    contentSourceTracker = new ContentSourceTracker();
    ContentSourceTracker* constTracker = contentSourceTracker;
    csClient->RegisterContentSourceListener(*constTracker);

    ContentClientParams* contentClientParams = new ContentClientParams();
    csClient->Init(contentClientParams);
    //End ContentServiceClient
}

int main(int argc, char**argv)
{
    // Allow CTRL+C to end application
    signal(SIGINT, signal_callback_handler);
    QCC_SetLogLevels("ALL=0");

    initServices();
    #ifdef _WIN32
    Sleep(1000);
    #else
    sleep(1);
    #endif

    menu = new Menu(contentSourceTracker);
    cout << "" << endl;
    while (menu->DisplayMenu()) {
        //NOOP
    }
    cleanup();

    return 1;
}

