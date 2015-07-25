/**
*Author: Steve Zhong
*Creation Date: 2015年07月25日 星期六 11时05分45秒
*Last Modified: 2015年07月25日 星期六 11时30分40秒
*Purpose: 历史数据共通定义 
**/

#ifndef HISTORY_H_
#define HISTORY_H_

#include <utility>
#include <string>

namespace dc {

using yahoo_se = std::pair<std::string, std::string>;

static const std::string BEGIN_DATE("1980-01-01"); 

}

#endif
