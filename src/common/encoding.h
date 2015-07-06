/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月06日 星期一 23时52分56秒
*Purpose:
**/

#ifndef ENCODING_H_
#define ENCODING_H_

#include <cstring>
#include <string>
#include <cstdlib>

#include <iconv.h>

#include <iostream>

namespace common {

class encoding {
public:
    static std::string gbk_to_utf8(std::string str) {
        size_t len = str.length();
        char *src = (char*) malloc(len);
        char *dest = (char*) calloc(1, len * 2);

        strcpy(src, str.c_str());
        charset_convert("GBK", "UTF-8", src, len, dest, len * 2);

        return std::string(dest);
    }
private:
    static int charset_convert(const char *from_charset,
                        const char *to_charset,
                        char *in_buf,
                        size_t in_left,
                        char *out_buf,
                        size_t out_left)
    {
        iconv_t icd;
        char *pin = in_buf;
        char *pout = out_buf;
        size_t out_len = out_left;
        if ((iconv_t)-1 == (icd = iconv_open(to_charset,from_charset))) {
            return -1;
        }
        if ((size_t)-1 == iconv(icd, &pin, &in_left, &pout, &out_left)) {
            iconv_close(icd);
            return -1;
        }
        out_buf[out_len - out_left] = 0;
        iconv_close(icd);
        return (int)out_len - out_left;
    }
};

}

#endif

