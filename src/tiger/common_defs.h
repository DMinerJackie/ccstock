/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月25日 星期四 22时36分49秒
*Purpose:
**/

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace tiger {

const char* CURR_P		= "现价";
const char* OPEN_P		= "今开";
const char* CLOSE_P		= "昨收";
const char* HIGH_P		= "最高";	
const char* LOW_P		= "最低";

const char* VOL			= "成交量";
const char* TO			= "成交额";

}

using get_code_func = std::function<void()>;
using gen_code_map  = std::unordered_map<std::string, get_code_func>;

#endif
