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

#ifndef CONTENTSOURCELISTENER_H_
#define CONTENTSOURCELISTENER_H_

namespace mdf {

class MdfContentSource;

class ContentSourceListener {

  public:

    /*
     * @function ContentSourceAdded
     * @description called when a new content source has been added
     * @param contentSource a reference to the content source that was added
     */
    virtual void ContentSourceAdded(MdfContentSource& contentSource) = 0;

    /*
     * @function ContentSourceLost
     * @description called when a new content source has been removed
     * @param contentSource a reference to the content source that was removed
     */
    virtual void ContentSourceLost(MdfContentSource& contentSource) = 0;

    /*
     * @function ~ContentSourceListener
     * @description virtual destructor
     */
    virtual ~ContentSourceListener() { }

};

} //END mdf

#endif /* CONTENSOURCELISTENER_H_ */
