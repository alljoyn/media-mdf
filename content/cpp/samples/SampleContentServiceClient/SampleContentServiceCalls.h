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

#ifndef __sampleContentServiceClient__SampleContentServiceCalls__
#define __sampleContentServiceClient__SampleContentServiceCalls__

#include <stdio.h>

#include "ContentProxyBusObject.h"

namespace mdf {

class SampleContentServiceCalls {

  public:

    SampleContentServiceCalls(ContentProxyBusObject* contentProxyBusObject);

    void TestAllCalls();

    void TestBrowse();

    void TestVersion();

    void TestDisplayName();

    void TestIconURL();

    void TestLastModified();

    void TestGetItem();

    void TestGetContentSourceInfo();

    void TestGetSortableFields();

    void TestModifyOptionalMetaData();

    void TestSearch();


  private:

    void Init();

    ContentProxyBusObject* m_ContentProxyBusObject;
    void PrintNodeList(NodeList* nodeList);
    void PrintNode(Node* node);

};


} //mdf
#endif /* defined(__sampleContentServiceClient__SampleContentServiceCalls__) */
