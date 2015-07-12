/**
*Author: Steve Zhong
*Creation Date: 2015年07月12日 星期日 18时05分07秒
*Last Modified: 2015年07月12日 星期日 18时31分21秒
*Purpose:
**/

#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

#include <string>

#include "common_defs.h"
#include "logger.h"

namespace common {

// 获取板块名称
const string get_bk_name(const std::string& bk)
{
    if (bk == "all") return all;
    else if (bk == "cyb") return cyb;
    else if (bk == "zxb") return zxb;
    else if (bk == "sz") return sz;
    else if (bk == "sh") return sh;
    else {
        logger::log_info("板块名称[" + bk + "]不存在!请检查");
        return "";
    }
}

}
#endif
