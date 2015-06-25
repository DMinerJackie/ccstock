/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月25日 星期四 22时35分13秒
*Purpose:
**/

#ifndef CMD_DISPLAY_H_
#define CMD_DISPLAY_H_

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "io_aux.h"
#include "common_defs.h"
#include "../muridae/stock.h"
#include "../cow/utility.h"

using namespace std;

namespace tiger {

class cmd_display {
public:
	using stock		= muridae::stock;
	using utility	= cow::utility;
	using self_type = cmd_display;

public:
	static void stock_info(const std::vector<stock>& stock_vec) {
		for (auto stock : stock_vec) {
			stock_info_one(stock);
		}
	}
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

private:
	static bool show_header(const stock& stock_) {
		io_aux::list_out("    ", "\n",
				{stock_.code, stock_.name, get_inc(stock_.get_inc())});
		return true;
	}
	static string process_volumn(const double& vol) {
		if (vol < 1e6) { return utility::to_string(vol / 1e2); }
		else { return utility::to_string_pcs(vol / 1e6, 2) + "万"; }
	}
	static string process_to(const double& to) {
		return (utility::to_string_pcs(to / 1e8, 2) + "亿");
	}
	static std::string get_inc(const double& inc) {
		if (inc > 0.0) { return "+" + utility::to_string_pcs(100 * inc, 2) + "%"; }
		else if (inc < 0.0) { return utility::to_string_pcs(100 * inc, 2) + "%"; }
		else { return "0.00"; }
	}
};

}
#endif
