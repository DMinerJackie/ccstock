/**
*Author: Steve Zhong
*Creation Date: 2015年07月19日 星期日 15时14分59秒
*Last Modified: 2015年07月19日 星期日 16时46分08秒
*Purpose:
**/

#ifndef YAHOO_HISTORY_CRAWLER_H_
#define YAHOO_HISTORY_CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>

#include <ev.h>
#include <curl/curl.h>

#include <common/common_func.h>
#include <common/logger.h>

#include "common_crawler.h"

namespace gateway {

class yahoo_history_crawler : public common_crawler {
public:
    using self_type = yahoo_history_crawler;
    using logger    = common::logger;
public:
    // 查看个股全部历史数据
    bool get_history_all(const cc_vec_string& code_vec, const cc_vec_string& file_name_vec)
    {
        for (size_t i = 0; i < code_vec.size(); ++i) {
            if ((i != 0 && i % 100 == 0) || i == code_vec.size() - 1) {
                logger::log_info("成功获取" + std::to_string(i + 1) + "个股票历史交易数据!");
            }
            set_code_vec({code_vec[i]});            
            download(file_name_vec[i].c_str(), std::bind(&self_type::get_qry_str,
                this));
        }
        return true;
    }
private:
    // 获取URL
    bool get_qry_str()
    {
        qry_str = "http://ichart.yahoo.com/table.csv?s=" + code_vec[0];        
        if (common::get_stock_type(code_vec[0]) == common::stock_type::SH) {
            qry_str += ".SS";
        }
        else {
            qry_str += ".SZ";
        }        
        return true;
    }
private:
};

}

#endif
