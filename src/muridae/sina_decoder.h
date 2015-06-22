/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 11时55分02秒
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
#include "../cow/encoding.h"
#include "../cow/utility.h"
#include "../cow/logger.h"

namespace muridae {

class sina_decoder {
public:
	using arr5			= std::array<double, 5>;
	using encoding		= cow::encoding;	
	using utility		= cow::utility;
	using logger		= cow::logger;

public:
	static bool decode(std::string& multi_md, std::vector<stock>& stock_vec) {
		std::vector<std::string> md_vec;
		utility::delete_char(multi_md, '\n');
		utility::split(multi_md, ';', md_vec, true);

		for (auto md : md_vec) {
			if (!decode_one(md, stock_vec)) { return false; }
		}
		
		return true;
	}
private:
	static bool decode_one(const std::string& data, std::vector<stock>& stock_vec) {
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
		
		stock_vec.push_back(stock_);	

		return true;
	}
};

}

#endif
