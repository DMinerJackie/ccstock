/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期日 10时55分24秒
*Last Modified: 2015年07月26日 星期日 11时50分15秒
*Purpose:
**/

#include "cctime.h"

namespace common {

// static成员变量定义
char timeutility::data_ptr[30];
time_t timeutility::rawtime;
tm* timeutility::timeinfo;

timeutility::timeutility(const std::string& date)
{
    strptime(date.c_str(), "%F", timeinfo);
}

std::string timeutility::add_day(size_t n)
{
    timeinfo->tm_mday += n;
    rawtime = mktime(timeinfo);
    timeinfo = localtime(&rawtime);
    strftime(data_ptr, 11, "%F", timeinfo);
    return std::string(data_ptr);
}

std::string timeutility::get_curr_date()
{
    curr_time();
    strftime(data_ptr, 11, "%F", timeinfo);
    return std::string(data_ptr);
}

std::string timeutility::get_curr_date_time()
{
    curr_time();
    strftime(data_ptr, 20, "%F %T", timeinfo);
    return std::string(data_ptr);
}

}

