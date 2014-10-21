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
#include "LogModule.h"

#include "SampleContentServiceClient.h"

static mdf::SampleContentServiceClient* sampleContentServiceClient = 0;


void cleanup()
{
    delete sampleContentServiceClient;
}

void signal_callback_handler(int32_t signum)
{
    cleanup();
    exit(signum);
}

int main(int argc, char* argv[])
{
    QCC_SetDebugLevel(mdf::CONTENT_SERVICE_LOG_NAME, mdf::ALL_LOG_LEVELS);
    QCC_DbgHLPrintf(("Starting the sample content service client"));
    // Allow CTRL+C to end application
    signal(SIGINT, signal_callback_handler);

    sampleContentServiceClient = new mdf::SampleContentServiceClient();
    QStatus status = sampleContentServiceClient->Init();
    if (status != ER_OK) {
        cleanup();
        return status;
    }

    QCC_DbgHLPrintf(("Completed sample content service client."));
    sampleContentServiceClient->RunExamples();

    int i = 0;
    while (i < 520) {
        sleep(1);
        i++;
    }
    cleanup();

    return 0;
}
