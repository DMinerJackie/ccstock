/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月27日 星期六 23时54分31秒
*Purpose:
**/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint> // int32_t, uint32_t

#include "crawler.h"
#include "stock.h"
#include "../simulator/cmd_display.h"
#include "../simulator/file_handler.h"
#include "../common/utility.h"
#include "../common/configurator.h"

namespace gateway {
	
class client {
public:
    using self_type     = client;
	using displayer		= simulator::cmd_display;
	using file_handler	= simulator::file_handler;
	using utility		= common::utility;
    using configurator  = common::configurator;
    using logger        = common::logger;
public:
    client(configurator* config_): config(config_)
    {
        gen_code_map_["sh"] = std::bind(&self_type::gen_code_vec_sh, this);
        gen_code_map_["sz"] = std::bind(&self_type::gen_code_vec_sz, this);
        gen_code_map_["cyb"] = std::bind(&self_type::gen_code_vec_cyb, this);
        gen_code_map_["zxb"] = std::bind(&self_type::gen_code_vec_zxb, this);
        gen_code_map_["all"] = std::bind(&self_type::gen_code_vec_all, this);
    }
	void show_md(const std::string& code_str)
	{
		crawler_.split_code(code_str);
		crawler_.run_code(stock_vec);
		display();
	}
    void show_market()
    {
        std::string sh_market = config->get_value("stock.market_data.code.sh_market", std::string());
        std::string sz_market = config->get_value("stock.market_data.code.sz_market", std::string());
        crawler_.show_market({sh_market, sz_market}, market_vec);
        display_market();
    }
    void show_md_bk(const std::string& bk, uint32_t speed, int32_t top_num, std::string& order)
    {
        std::string path = config->get_value("stock.market_data.code.code_path", std::string());
        file_handler::read_code(code_vec, path, bk);
        std::vector<std::string>sub_code;
        uint32_t idx = 0;
        while (utility::get_vec_by_step(code_vec, idx, speed, sub_code)) {
            crawler_.list_stock(sub_code, stock_vec);
            logger::log_debug_variadic("get stock ", idx - speed, "-", idx, " finished!");
        }
        if (!sub_code.empty()) {
            crawler_.list_stock(sub_code, stock_vec);
        }
        select_stock(top_num, order);
        display_basic(top_num);
    }
    void add_option(const std::string& options)
    {
        std::vector<std::string> code_vec;
        utility::split(options, ',', code_vec);
        std::string option_path = config->get_value("stock.market_data.code.option_path", std::string());
        file_handler::add_option(code_vec, option_path); 
    }
    void del_option(const std::string& options)
    {
        std::vector<std::string> code_vec;
        utility::split(options, ',', code_vec);
        std::string option_path = config->get_value("stock.market_data.code.option_path", std::string());
        file_handler::del_option(code_vec, option_path); 
    }
    void show_option()
    {
        std::vector<std::string> code_vec;
        std::string option_path = config->get_value("stock.market_data.code.option_path", std::string());
        file_handler::read_option(code_vec, option_path);
        crawler_.list_stock(code_vec, stock_vec);
        display_basic();
    }
public:
    void gen_code(const std::string& bk)
    {
        code_vec.clear();
        gen_code_map_[bk]();
        gen_code_name(bk);
    }
private:
    bool select_stock(int32_t top_num, std::string& order)
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
        return true;
    }
	void gen_code_name(const std::string& bk)
	{          
        crawler_.list_code_name(code_vec, stock_vec);
        std::string path = config->get_value("stock.market_data.code.code_path", std::string());
		file_handler::save_code_name(stock_vec, path, bk + "_code_name.ds");
	}
	void gen_code_vec_sh()
	{
        int beg = config->get_value("stock.market_data.code.sh.beg", int());
        int end = config->get_value("stock.market_data.code.sh.end", int());
        utility::gen_range_str(beg, end, code_vec);
	}
	void gen_code_vec_sz()
	{
        gen_code_vec_zxb();
        gen_code_vec_cyb();
        int beg = config->get_value("stock.market_data.code.sz.beg", int());
        int end = config->get_value("stock.market_data.code.sz.end", int());
        utility::gen_range_str(beg, end, code_vec);
        config->get_multi_value("stock.market_data.code.sz.specials", code_vec);
    }
	void gen_code_vec_zxb()
	{
        int beg = config->get_value("stock.market_data.code.sz.zxb.beg", int());
        int end = config->get_value("stock.market_data.code.sz.zxb.end", int());
        utility::gen_range_str(beg, end, code_vec);
	}
	void gen_code_vec_cyb()
    {
        int beg = config->get_value("stock.market_data.code.sz.cyb.beg", int());
        int end = config->get_value("stock.market_data.code.sz.cyb.end", int());
        utility::gen_range_str(beg, end, code_vec);
	}
	void gen_code_vec_all()
	{
        gen_code_vec_sh();
        gen_code_vec_sz();
	}
private:
	void display() {
		displayer::stock_info(stock_vec);
	}
    void display_market() {
        displayer::market_info(market_vec);
    }
    void display_basic(int32_t top_num = -1) {
        if (top_num == -1) {
            displayer::stock_info_basic(stock_vec);
        } else {
            displayer::stock_info_basic(vector<stock>(stock_vec.begin(), stock_vec.begin() + top_num));
        }
    }
private:
	crawler crawler_;
	std::vector<std::string> code_vec;
	std::vector<stock> stock_vec;
	std::vector<market> market_vec;
    gen_code_map gen_code_map_;
    std::shared_ptr<configurator> config;
};

}
#endif
