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

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <ostream>
#include <sstream>
#include <string>
#include <qcc/String.h>

namespace mdf {

class StringUtils {

  public:

    template <typename T>
    static T StringToNumber(const qcc::String& Text)
    {
        std::istringstream ss(Text.c_str());
        T result;
        return ss >> result ? result : 0;
    }

    template <typename T>
    static qcc::String NumberToString(T Number)
    {
        std::ostringstream ss;
        ss << Number;
        return qcc::String(ss.str().c_str());
    }

};

} //end mdf

#endif /* STRINGUTILS_H_ */
