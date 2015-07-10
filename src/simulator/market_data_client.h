/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月09日 星期四 11时02分51秒
*Purpose:
**/

#ifndef MARKET_DATA_CLIENT_H_
#define MARKET_DATA_CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint> // int32_t, uint32_t

#include "instrument/stock.h"

#include "simulator/cmd_display.h"
#include "simulator/file_handler.h"

#include "common/utility.h"
#include "common/io_aux.h"
#include "common/configurator.h"
#include "common/logger.h"
#include "common/common_defs.h"

namespace simulator {

template <typename code_db, typename md_crawler>
class market_data_client {
public:
    using self_type     = market_data_client<code_db, md_crawler>;
    using displayer     = simulator::cmd_display;
    using file_handler  = simulator::file_handler;
    using utility       = common::utility;
    using configurator  = common::configurator;
    using logger        = common::logger;
    using io_aux        = common::io_aux;
public:
    market_data_client(const shared_ptr<configurator>& config_):
        config(config_),
        db(new code_db())
    {
    }
    void initialize()
    {
        code_path = config->get_value("stock.market_data.code.code_path", std::string());
        option_path = config->get_value("stock.market_data.code.option_path", std::string());
        db->configure(code_path);
    }
    // 查看个股信息
    void show_stock(const std::string& code_str)
    {
        code_vec.clear();
        utility::split(code_str, ',', code_vec);
        md_crawler_.get_stock_data(code_vec, 
                std::bind(&self_type::display_stock,
                    this,
                    std::placeholders::_1));
        md_crawler_.run();
    }
    // 查看大盘行情
    void show_market()
    {
        cc_vec_string market_codes;
        config->get_multi_value("stock.market_data.code.markets", market_codes);
        md_crawler_.get_market_data(market_codes,
                std::bind(&self_type::display_market,
                    this,
                    std::placeholders::_1));
        md_crawler_.run();
    }
    // 查看板块行情
    void show_md_bk(const std::string& bk, uint32_t speed, int32_t top_num, std::string& order)
    {
        code_vec.clear();
        file_handler::read_code(code_vec, code_path, bk);
        md_crawler_.get_bk_data(code_vec, speed, top_num, order,
                std::bind(&self_type::display_stock, this, std::placeholders::_1),
                std::bind(&self_type::select_stock, this, std::placeholders::_1, 
                    std::placeholders::_2,
                    std::placeholders::_3));
        md_crawler_.run();
    }
private:
    bool select_stock(int32_t top_num, std::string& order, std::vector<stock>& stock_vec)
    {
        if (top_num == -1) top_num = stock_vec.size();

        if (order == "") { }
        else if (order == "zdf") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.inc > rsh.inc; });
        }
        else if (order == "xj") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.curr_price > rsh.curr_price; });
        }
        else if (order == "cjl") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.volumn > rsh.volumn; });
        }
        else if (order == "cje") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.turnover > rsh.turnover; });
        }
        else if (order == "zdfi") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.inc < rsh.inc; });
        }
        else if (order == "xji") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.curr_price < rsh.curr_price; });
        }
        else if (order == "cjli") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.volumn < rsh.volumn; });
        }
        else if (order == "cjei") {
            std::partial_sort(stock_vec.begin(),
                    stock_vec.begin() + top_num,
                    stock_vec.end(),
                    [](const stock& lsh, const stock& rsh) { return lsh.turnover < rsh.turnover; });
        }
        else {
            logger::log_error("排序标准有问题!");
            return false;
        }
        stock_vec = std::vector<stock>(stock_vec.begin(), stock_vec.begin() + top_num);
        return true;
    }
private:
    void display_stock(std::vector<stock>& stock_vec) {
        displayer::stock_info(stock_vec);
    }
    void display_market(std::vector<market>& market_vec) {
        displayer::market_info(market_vec);
    }
private:
    md_crawler md_crawler_;
    cc_vec_string code_vec;

    std::string code_path; // 股票代码文件存放
    std::string option_path; // 自选股代码存放

    std::vector<stock> stock_vec;
    std::vector<market> market_vec;
    gen_code_map gen_code_map_;
    std::shared_ptr<configurator> config;
private:
    code_db* db;
};

}
#endif
