/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 20时00分08秒
*Last Modified: 2015年07月24日 星期五 22时00分14秒
*Purpose: 日期类封装
**/

#ifndef DATATIME_H_
#define DATATIME_H_

#include <string>
#include <ctime>

namespace common {

class datetime {
public:
    datetime() { }
    datetime(unsigned short year_, unsigned char month_, unsigned char day_)
        : year(year_), month(month_), day(day_)
    { }
    unsigned short get_year() { return year; }
    unsigned short get_month() { return month; }
    unsigned short get_day() { return day; }

    void set_year(unsigned short year_) { year = year_; }
    void set_month(unsigned char month_) { month = month_; }
    void set_day(unsigned char day_) { day = day_; }

    std::string date_to_string() {
        return std::to_string(year) + "-"
            + std::to_string(month) + "-"
            + std::to_string(day);
    }
public:
    std::string get_curr_time()
    {
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(date_ptr, 11, "%F", timeinfo);
        return std::string(date_ptr);
    }
private:
    char date_ptr[11]; // 存储日期
private:
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};

}

#endif
