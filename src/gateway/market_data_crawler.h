/**
*Author: Steve Zhong
*Creation Date: 2015年07月06日 星期一 19时56分34秒
*Last Modified: 2015年07月19日 星期日 15时39分04秒
*Purpose:
**/

#ifndef MARKET_DATA_CRAWLER_H_
#define MARKET_DATA_CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>

#include <ev.h>
#include <curl/curl.h>

#include <common/common_func.h>
#include <common/utility.h>
#include <common/logger.h>
#include <simulator/instrument/stock.h>

#include "sina_decoder.h"
#include "common_crawler.h"

namespace gateway {

class market_data_crawler : public common_crawler {
    using utility       = common::utility;
private:
    struct ev_timer_wrapper {
        ev_timer timeout_watcher;
        market_data_crawler* crawler_;
        bool all; // 是否显示停牌股票
    };
    struct ev_timer_wrapper_bk : ev_timer_wrapper {
        uint32_t speed;
        uint32_t top_num;
        std::string order;
        const cc_vec_string* code_vec;
        std::string bk;
    };
public:
    using self_type = market_data_crawler;
    using decoder   = sina_decoder;
    using stock     = simulator::stock;
    using stock_basic = simulator::stock_basic;
    using market    = simulator::market;
    using logger    = common::logger;
public:
    market_data_crawler()
    {
        ev_loop_ = EV_DEFAULT;
    }
    // 开始事件循环
    void run()
    {
        ev_run(ev_loop_, 0);
    }
    // 查看个股信息
    bool get_stock_data(const cc_vec_string& code_vec, display_stock_cb_t display_stock_cb_, bool all = false)
    {
        set_code_vec(code_vec);
        display_stock_cb = display_stock_cb_;
        timer_wrapper.crawler_ = this;
        timer_wrapper.all = all;
        ev_timer_init(&timer_wrapper.timeout_watcher, stock_timeout_cb, 0.5, 1);
        ev_timer_start(ev_loop_, &timer_wrapper.timeout_watcher);
        return true;
    }
    // 查看大盘行情
    bool get_market_data(const cc_vec_string& code_vec, display_market_cb_t display_market_cb_)
    {
        set_code_vec(code_vec);
        display_market_cb = display_market_cb_;
        timer_wrapper.crawler_ = this;
        ev_timer_init(&timer_wrapper.timeout_watcher, market_timeout_cb, 1.0, 1);
        ev_timer_start(ev_loop_, &timer_wrapper.timeout_watcher);
        return true;
    }
    // 查看板块信息
    bool get_bk_data(const string& bk, const cc_vec_string& code_vec, uint32_t speed, int32_t top_num, std::string& order,
            display_stock_cb_t display_stock_cb_,
            select_stock_cb_t select_stock_cb_)
    {
        set_code_vec(code_vec);
        display_stock_cb = display_stock_cb_;
        select_stock_cb = select_stock_cb_;
        timer_wrapper_bk.crawler_ = this;
        timer_wrapper_bk.speed = speed;
        timer_wrapper_bk.top_num = top_num;
        timer_wrapper_bk.order = order;
        timer_wrapper_bk.code_vec = &code_vec;
        timer_wrapper_bk.bk = bk;

        ev_timer_init(&timer_wrapper_bk.timeout_watcher, bk_timeout_cb, 1.0, 1);
        ev_timer_start(ev_loop_, &timer_wrapper_bk.timeout_watcher);
        return true;
    }
private:
    static void stock_timeout_cb(EV_P_ ev_timer *w, int)
    {
        ev_timer_wrapper *wrapper = (ev_timer_wrapper*) w;
        market_data_crawler* crawler_ = wrapper->crawler_;
        string stock_data;
        std::vector<stock> stock_vec;
        crawler_->crawler_content(stock_data, std::bind(&self_type::get_qry_str,
                    crawler_));
        decoder::decode(stock_data, stock_vec, wrapper->all);
        crawler_->display_stock_cb(stock_vec, "A股个股行情");
    }
    static void bk_timeout_cb(EV_P_ ev_timer *w, int)
    {
        ev_timer_wrapper_bk *wrapper = (ev_timer_wrapper_bk*) w;
        market_data_crawler* crawler_ = wrapper->crawler_;
        uint32_t speed = wrapper->speed;
        uint32_t idx = 0;
        std::vector<stock> stock_vec;
        string bk_data;

        cc_vec_string subvec;
        while (utility::get_vec_by_step(*(wrapper->code_vec), idx, speed, subvec)) {
            bk_data = "";
            crawler_->set_code_vec(subvec);
            crawler_->crawler_content(bk_data, std::bind(&self_type::get_qry_str,
                    crawler_));
            decoder::decode(bk_data, stock_vec, true);
        }
        if (!subvec.empty()) {
            bk_data = "";
            crawler_->set_code_vec(subvec);
            crawler_->crawler_content(bk_data, std::bind(&self_type::get_qry_str,
                    crawler_));
            decoder::decode(bk_data, stock_vec, true);
        }
        crawler_->select_stock_cb(wrapper->top_num, wrapper->order, stock_vec);
        crawler_->display_stock_cb(stock_vec, common::get_bk_name(wrapper->bk) + "个股行情");
    }
    static void market_timeout_cb(EV_P_ ev_timer *w, int)
    {
        ev_timer_wrapper *wrapper = (ev_timer_wrapper*) w;
        market_data_crawler* crawler_ = wrapper->crawler_;
        string market_data;
        std::vector<market> market_vec;
        crawler_->crawler_content(market_data, std::bind(&self_type::get_plain_qry_str,
                    crawler_));
        decoder::decode_market(market_data, market_vec);
        crawler_->display_market_cb(market_vec);
    }
private:
    // 获取URL列表-未加上sh/sz前缀
    bool get_qry_str() {
        qry_str = "http://hq.sinajs.cn/list=";
        bool first = false;
        for (auto code : code_vec) {
            if (code.length() != 6) {
                logger::code_error(code);
                return false;
            }
            if (first)  { qry_str += ','; }
            else { first = true; }
            if (code[0] == '0' || code[0] == '3') {
                qry_str += "sz" + code;
            } else if (code[0] == '6') {
                qry_str += "sh" + code;
            } else {
                logger::code_error(code);
                return false;
            }
        }
        return true;
    }
    // 获取URL列表-已加上sh/sz前缀
    bool get_plain_qry_str()
    {
        qry_str = "http://hq.sinajs.cn/list=";
        for (auto code : code_vec) {
            qry_str += code + ",";
        }
        qry_str = qry_str.substr(0, qry_str.size() - 1);
        return true;
    }
private:
    std::vector<stock> stock_vec;

    struct ev_loop *ev_loop_;
    ev_timer_wrapper timer_wrapper;
    ev_timer_wrapper_bk timer_wrapper_bk;

    display_stock_cb_t display_stock_cb;
    display_market_cb_t display_market_cb;
    select_stock_cb_t select_stock_cb;
};

}

#endif
