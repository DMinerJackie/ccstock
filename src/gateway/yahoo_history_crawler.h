/**
*Author: Steve Zhong
*Creation Date: 2015年07月19日 星期日 15时14分59秒
*Last Modified: 2015年07月30日 星期四 20时19分07秒
*Purpose:
**/

#ifndef YAHOO_HISTORY_CRAWLER_H_
#define YAHOO_HISTORY_CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>
#include <functional>

#include <ev.h>
#include <curl/curl.h>

#include <common/common_func.h>
#include <common/logger.h>
#include <common/system/cctime.h>

#include "common_crawler.h"

namespace gateway {

struct user_data {
    std::string code;
    common::timewrapper endtime;
};

class yahoo_history_crawler : public common_crawler {
public:
    using self_type = yahoo_history_crawler;
    using logger    = common::logger;
public:
    // 设置api地址
    void configure(const std::string& api_)
    {
        api = api_;
    }
    // 查看个股部分历史数据
    bool get_history_inc(const cc_vec_string& code_vec,
            std::vector<common::timewrapper>& endtime_vec,
            const cc_vec_string& file_name_vec)
    {
        user_data user_data_;
        for (size_t i = 0; i < code_vec.size(); ++i) {
            if ((i != 0 && i % 100 == 0) || i == code_vec.size() - 1) {
                logger::log_info("成功获取" + std::to_string(i) + "个股票历史交易数据!");
            }
            // 传递用户数据
            user_data_.code = code_vec[i];
            user_data_.endtime = endtime_vec[i];
            if (download(file_name_vec[i].c_str(),
                    std::bind(&self_type::get_range_qry_str, this, std::placeholders::_1),
                    (void *) &user_data_))
            {
            }
        }
        return true;
    }
private:
    bool get_qry_str() { return true; }
    // 获取URL, 某个区间a:month, b:day, c:year
    bool get_range_qry_str(void* user_ptr)
    {
        user_data* user_data_ = (user_data*) user_ptr;
        user_data_->endtime.add_day(1);
        // 拼接api地址
        qry_str = api + user_data_->code;
        qry_str += common::get_stock_type(user_data_->code) == common::stock_type::SH ? ".SS" : ".SZ";
        qry_str += string("&g=d&")
            + "a=" + std::to_string(user_data_->endtime.get_month() - 1) + "&"
            + "b=" + std::to_string(user_data_->endtime.get_day()) + "&"
            + "c=" + std::to_string(user_data_->endtime.get_year()) + "&"
            + "ignore=.csv";
        logger::log_info(qry_str);
        return true;
    }
private:
    string api;
};

}

#endif
