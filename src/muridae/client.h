/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月25日 星期四 22时23分41秒
*Purpose:
**/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "crawler.h"
#include "stock.h"
#include "../tiger/cmd_display.h"
#include "../tiger/file_handler.h"
#include "../cow/utility.h"
#include "../cow/configurator.h"

namespace muridae {
	
class client {
public:
    using self_type     = client;
	using displayer		= tiger::cmd_display;
	using file_handler	= tiger::file_handler;
	using utility		= cow::utility;
    using configurator  = cow::configurator;
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
public:
    void gen_code(const std::string& bk)
    {
        code_vec.clear();
        gen_code_map_[bk]();
        gen_code_name(bk);
    }
private:
	void gen_code_name(const std::string& bk)
	{
		crawler_.list_code_name(code_vec, stock_vec);
		file_handler::save_code_name(stock_vec, bk + "_code_name.ds");
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
private:
	crawler crawler_;
	std::vector<std::string> code_vec;
	std::vector<stock> stock_vec;
    gen_code_map gen_code_map_;
    std::shared_ptr<configurator> config;
};

}
#endif
