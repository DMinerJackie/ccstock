/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 20时00分08秒
*Last Modified: 2015年07月26日 星期日 12时18分36秒
*Purpose: 日期类封装
**/

#ifndef CCTIME_H_
#define CCTIME_H_

#include <string>
#include <ctime>

namespace common {

using time_adt = tm*;

// 时间封装类
class timeutility {
public:
    timeutility() { }
    // 通过YYYY-MM-DD构造
    timeutility(const std::string& date);
public:
    // 时间增加几天
    std::string add_day(size_t n);
public:
    // 获取当前系统日期YYYY-MM-DD
    static std::string get_curr_date();
    // 获取当前系统日期和时间YYYY-MM-DD HH:MM:SS
    static std::string get_curr_date_time();
private:
    // 获取当前时间和日期
    static void curr_time() {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
    }
private:
    static char data_ptr[30];
    static time_t rawtime;
    static tm* timeinfo;
};

// 未完成。。。
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
