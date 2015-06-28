/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月27日 星期六 16时52分30秒
*Purpose:
**/

#ifndef SINA_DECODER_H
#define SINA_DECODER_H

#include <string>
#include <array>
#include <vector>
#include <utility>
#include <stddef.h> // size_t

#include "stock.h"
#include "../common/encoding.h"
#include "../common/utility.h"
#include "../common/logger.h"

namespace gateway {

class sina_decoder {
public:
	using arr5			= std::array<double, 5>;
	using encoding		= common::encoding;	
	using utility		= common::utility;
	using logger		= common::logger;

public:
	static bool decode(std::string& multi_md, std::vector<stock>& stock_vec) {
		std::vector<std::string> md_vec;
		utility::delete_char(multi_md, '\n');
		utility::split(multi_md, ';', md_vec, true);

		for (auto md : md_vec) {
			if (!decode_one(md, stock_vec)) { continue; }
		}
		
		return true;
	}
	static bool decode_market(std::string& multi_md, std::vector<market>& market_vec) {
		std::vector<std::string> md_vec;
		utility::delete_char(multi_md, '\n');
		utility::split(multi_md, ';', md_vec, true);

		for (auto md : md_vec) {
			if (!decode_market_one(md, market_vec)) { continue; }
		}
		
		return true;
	}
private:
	static bool decode_one(const std::string& data, std::vector<stock>& stock_vec, bool all = false) {
		stock stock_;
		stock_.code = data.substr(13, 6);

		std::string md;
		utility::subsplit(data, '"', md);

		if (md == "") {
			logger::code_error(stock_.code);
			return false;
		}

		std::vector<std::string> field;
		utility::split(md, ',', field);
	
		stock_.name			= encoding::gbk_to_utf8(field[0]);
		stock_.open_price	= std::stod(field[1]);
		stock_.close_price	= std::stod(field[2]);
		stock_.curr_price	= std::stod(field[3]);
		stock_.high_price	= std::stod(field[4]);
		stock_.low_price	= std::stod(field[5]);
		
		stock_.buy_price[0]		= std::stod(field[6]);
		stock_.sell_price[0]	= std::stod(field[7]);

		stock_.volumn	= std::stod(field[8]);
		stock_.turnover = std::stod(field[9]);

		stock_.buy_qty[0]	= std::stod(field[10]);
		stock_.buy_price[0] = std::stod(field[11]);
		stock_.buy_qty[1]	= std::stod(field[12]);
		stock_.buy_price[1] = std::stod(field[13]);
		stock_.buy_qty[2]	= std::stod(field[14]);
		stock_.buy_price[2] = std::stod(field[15]);
		stock_.buy_qty[3]	= std::stod(field[16]);
		stock_.buy_price[3] = std::stod(field[17]);
		stock_.buy_qty[4]	= std::stod(field[18]);
		stock_.buy_price[4] = std::stod(field[19]);
		
		stock_.sell_qty[0]		= std::stod(field[20]);
		stock_.sell_price[0]	= std::stod(field[21]);
		stock_.sell_qty[1]		= std::stod(field[22]);
		stock_.sell_price[1]	= std::stod(field[23]);
		stock_.sell_qty[2]		= std::stod(field[24]);
		stock_.sell_price[2]	= std::stod(field[25]);
		stock_.sell_qty[3]		= std::stod(field[26]);
		stock_.sell_price[3]	= std::stod(field[27]);
		stock_.sell_qty[4]		= std::stod(field[28]);
		stock_.sell_price[4]	= std::stod(field[29]);

		stock_.date = field[30];
		stock_.time = field[31];
		
	    stock_.inc =  (stock_.curr_price - stock_.close_price) / stock_.close_price;

        if (stock_.curr_price != 0.0 || all)
		    stock_vec.push_back(stock_);	

		return true;
	}
    static bool decode_market_one(const std::string& data, std::vector<market>& market_vec)
    {
		std::string md;
		utility::subsplit(data, '"', md);

		std::vector<std::string> field;
		utility::split(md, ',', field);
        market market_;
        market_.name        = encoding::gbk_to_utf8(field[0]);
        market_.index       = std::stod(field[1]);
        market_.inc_qty     = std::stod(field[2]);
        market_.inc_per     = std::stod(field[3]);
        market_.volumn      = std::stod(field[4]);
        market_.turnover    = std::stod(field[5]);
        market_vec.push_back(market_);
        return true;
    }
};

}

#endif
