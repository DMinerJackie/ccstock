/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月06日 星期一 23时54分03秒
*Purpose:
**/

#ifndef CMD_DISPLAY_H_
#define CMD_DISPLAY_H_

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "common/io_aux.h"
#include "common/common_defs.h"
#include "instrument/stock.h"
#include "common/utility.h"

using namespace std;
using namespace common;

namespace simulator {

class cmd_display {
public:
    using utility   = common::utility;
    using self_type = cmd_display;

public:
    static void stock_info(const std::vector<stock>& stock_vec) {
        for (auto stock : stock_vec) {
            stock_info_one(stock);
        }
    }
    static void stock_info_basic(const std::vector<stock>& stock_vec) {
        for (auto stock : stock_vec) {
            stock_info_one_basic(stock);
        }
    }
    static void market_info(const std::vector<market>& market_vec)
    {
        for (auto market_ : market_vec) {
            market_info_one(market_);
        }
    }
private:
    static void stock_info_one(const stock& stock_) {
        self_type::show_header(stock_);
        io_aux::list_pair_out("\t", "\t",
                {CURR_P, OPEN_P, CLOSE_P, HIGH_P, LOW_P},
                {stock_.curr_price,
                 stock_.open_price,
                 stock_.close_price,
                 stock_.high_price,
                 stock_.low_price});
        io_aux::list_pair_out("\t   ", "\n",
                {VOL, TO},
                {process_volumn(stock_.volumn).c_str(), process_to(stock_.turnover).c_str()});
        io_aux::divider();
    }
    static void stock_info_one_basic(const stock& stock_) {
        self_type::show_header(stock_);
        io_aux::list_pair_out("\t", "\t",
                {CURR_P},
                {stock_.curr_price});
        io_aux::list_pair_out("\t   ", "\n",
                {VOL, TO},
                {process_volumn(stock_.volumn).c_str(), process_to(stock_.turnover).c_str()});
        io_aux::divider();
    }
    static void market_info_one(const market& market_) {
        io_aux::list_out("    \t", "\n",
                {market_.name,
                utility::to_string_pcs(market_.index, 2),
                utility::to_string_pcs(market_.inc_qty, 2),
                utility::to_string_pcs(market_.inc_per, 2) + "%",
                process_volumn_market(market_.volumn),
                process_to_market(market_.turnover)});
    }
private:
    static bool show_header(const stock& stock_) {
        io_aux::list_out({10, 16, 11},
                "",
                {stock_.code, stock_.name, get_inc(stock_.inc)});
        return true;
    }
    static std::string process_volumn(const double& vol) {
        if (vol < 1e6) { return utility::to_string((uint32_t)(vol / 1e2)); }
        else { return utility::to_string_pcs(vol / 1e6, 2) + "万"; }
    }
    static std::string process_to(const double& to) {
        return (utility::to_string_pcs(to / 1e8, 2) + "亿");
    }
    static std::string process_volumn_market(const double& vol) {
        return utility::to_string_pcs(vol / 1e4, 2) + "万";
    }
    static std::string process_to_market(const double& to) {
        return (utility::to_string_pcs(to / 1e4, 2) + "亿");
    }
    static std::string get_inc(const double& inc) {
        if (inc > 0.0) { return "+" + utility::to_string_pcs(100 * inc, 2) + "%"; }
        else if (inc < 0.0) { return utility::to_string_pcs(100 * inc, 2) + "%"; }
        else { return "0.00"; }
    }
};

}
#endif
