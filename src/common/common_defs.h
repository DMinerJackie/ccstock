/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月19日 星期日 15时54分58秒
*Purpose:
**/

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace common {

const char* CODE_CH     = "股票代码";
const char* NAME_CH     = "股票名称";

const char* CURR_P      = "现价";
const char* OPEN_P      = "今开";
const char* CLOSE_P     = "昨收";
const char* HIGH_P      = "最高";
const char* LOW_P       = "最低";

const char* VOL         = "成交量";
const char* TO          = "成交额";

const char* INC         = "涨幅";
const char* INC_V       = "涨跌";

const std::string all = "A股";
const std::string cyb = "创业板";
const std::string zxb = "中小板";
const std::string sz  = "深市";
const std::string sh  = "沪市";

enum class stock_type { SH, SZ };

}

using gen_qry_str_func = std::function<bool()>;

using get_code_func = std::function<void()>;
using gen_code_map  = std::unordered_map<std::string, get_code_func>;

using cc_vec_string = std::vector<std::string>;

#endif
