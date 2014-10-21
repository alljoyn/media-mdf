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
#include <signal.h>
#include "SampleContentService.h"
#include "MdfContentSource.h"
#include "SampleContentSource.h"
#include <alljoyn/PasswordManager.h>
#include <alljoyn/about/AboutServiceApi.h>
#include <SrpKeyXListener.h>
#include <alljoyn/services_common/LogModulesNames.h>
#include <LogModule.h>


#include <qcc/String.h>
#include <qcc/Log.h>

#include "SampleContentService.h"


using namespace mdf;

SampleContentService* sampleContentService = 0;

void cleanup()
{
    // Clean up
    if (sampleContentService) {
        delete sampleContentService;
    }
}

void signal_callback_handler(int32_t signum)
{
    cleanup();
    exit(signum);
}

int main(int argc, char* argv[])
{
    QCC_SetDebugLevel(CONTENT_SERVICE_LOG_NAME, ALL_LOG_LEVELS);

    // Allow CTRL+C to end application
    signal(SIGINT, signal_callback_handler);

    QCC_DbgHLPrintf(("Starting SampleContentService"));

    sampleContentService = new SampleContentService();
    sampleContentService->Init();

    QCC_DbgHLPrintf(("Finished SampleContentService startup. Waiting for clients to appear."));

    int i = 0;
    while (i < 520) {
        sleep(1);
        i++;
    }

    cleanup();

    return 0;
}
