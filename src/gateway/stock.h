/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月02日 星期四 00时03分23秒
*Purpose:
**/

#ifndef STOCK_H_
#define STOCK_H_

#include <string>
#include <array>
#include <utility>
#include <cstddef>

namespace gateway {

class stock {
public:
	using arr5 = std::array<double, 5>;

public:
//	inline std::string get_name() const { return name; }
//	inline std::string get_code() const { return code; }
//	inline double get_open_price() const { return open_price; }
//	inline double get_close_price() const { return close_price; }
	inline std::pair<arr5, arr5> get_buyer() const { return std::make_pair(buy_price, buy_qty); }
	inline std::pair<arr5, arr5> get_seller() const { return std::make_pair(sell_price, sell_qty); }
//	inline std::string get_date() const { return data; }
//	inline std::string get_time() const { return time; }

public:
	double get_inc() const { return (curr_price - close_price) / close_price; }
public:
	std::string name;
	std::string code;
	double open_price;
	double close_price;
	double high_price;
	double low_price;
	double curr_price;
	double volumn; // 成交量
	double turnover; // 成交金额

	arr5 buy_price;
	arr5 buy_qty;
	arr5 sell_price;
	arr5 sell_qty;

	std::string date;
	std::string time;
    
    double inc; // 涨跌幅
};

struct stock_basic {
    std::string name;
    std::string jp; // 简拼
    std::string code; // 代码
    std::string market_code; // 加sz/sh的代码
};

struct market {
    std::string name;
    double index;
    double inc_qty;
    double inc_per;
    double volumn;
    double turnover;
};

}

#endif
