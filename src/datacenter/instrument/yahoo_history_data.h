/**
*Author: Steve Zhong
*Creation Date: 2015年07月30日 星期四 19时27分12秒
*Last Modified: 2015年07月31日 星期五 00时04分05秒
*Purpose: yahoo历史数据定义 
**/

#ifndef YAHOO_HISTORY_DATE_H_
#define YAHOO_HISTORY_DATE_H_

#include <string>

namespace dc {
namespace inst {

class yahoo_hist {
public:
    yahoo_hist() { }
    yahoo_hist(const std::string& date_, double open_, double high_, double low_, double close_, double volumn_, double adj_close_)
        : date(date_), open(open_), high(high_), low(low_), close(close_), volumn(volumn_), adj_close(adj_close_) { }
public:
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volumn;
    double adj_close; // 复权收盘价
};

}
}


#endif
