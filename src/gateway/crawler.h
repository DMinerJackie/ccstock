/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月05日 星期日 00时47分22秒
*Purpose:
**/
#ifndef CRAWLER_H_
#define CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>

#include <curl/curl.h>

#include "sina_decoder.h"
#include "simulator/instrument/stock.h"

#include "common/logger.h"
#include "common/utility.h"
#include "common/common_defs.h"

namespace gateway {

class crawler {
public:
    using self_type = crawler;
	using decoder	= sina_decoder;
    using stock     = simulator::stock;
    using stock_basic = simulator::stock_basic;
    using market 	= simulator::market;
	using logger	= common::logger;
	using utility	= common::utility;
public:
	crawler() {}

	bool split_code(const std::string& code_str)
    {
		utility::split(code_str, ',', code_vec);
		return true;
	}

	bool run_code(std::vector<stock>& stock_vec)
    {
		string stock_data;
        crawler_content(stock_data, std::bind(&self_type::get_qry_str,
                    this,
                    std::placeholders::_1));
    	if(!decoder::decode(stock_data, stock_vec)) { return false; }
		return true;
	}

    bool list_code(cc_vec_string& init_code, cc_vec_string& final_code)
    {
        set_code_vec(init_code);
        std::string code_data;
        crawler_content(code_data, std::bind(&self_type::get_simple_qry_str,
                this,
                std::placeholders::_1));
    	decoder::decode_code(code_data, final_code);
        return true;
    }
    bool list_stock(const cc_vec_string& code_vec, std::vector<stock>& stock_vec)
    {
        set_code_vec(code_vec);
        run_code(stock_vec);
        return true;
    }
    bool gen_code_jp_name(const cc_vec_string& code_vec, std::vector<stock_basic>& code_jp_name_vec)
    {
        code_jp_name_idx = 0;
        set_code_vec(code_vec);
        std::string code_jp_name_data;
        for (auto code : code_vec) {
            crawler_content(code_jp_name_data, std::bind(&self_type::get_code_jp_name_qry_str,
                    this,
                    std::placeholders::_1));
            decoder::decode_code_jp_name(code_jp_name_data, code_jp_name_vec);
            if (++code_jp_name_idx % 200 == 0) {
                logger::log_debug_variadic("获取股票简拼数量(比较慢): ", code_jp_name_idx);
            }
            code_jp_name_data = "";
        }
        return true;
    }
    bool show_market(const cc_vec_string& code_vec, std::vector<market>& market_vec)
    {
        set_code_vec(code_vec);
        string market_data;
        crawler_content(market_data, std::bind(&self_type::get_plain_qry_str,
                    this,
                    std::placeholders::_1));
        decoder::decode_market(market_data, market_vec);
        return true;
    }
private:
    bool set_code_vec(const cc_vec_string& code_vec_)
    {
        this->code_vec = code_vec_;
        return true;
    }
	inline bool get_qry_str(string& qry_str) {
		qry_str += "http://hq.sinajs.cn/list=";

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
	inline bool get_simple_qry_str(string& qry_str) {
		qry_str += "http://hq.sinajs.cn/list=";
		bool first = false;
		for (auto code : code_vec) {
			if (code.length() != 6) {
				logger::code_error(code);
				return false;
			}
			if (first)  { qry_str += ','; }
			else { first = true; }
			if (code[0] == '0' || code[0] == '3') {
				qry_str += "s_sz" + code;
			} else if (code[0] == '6') {
				qry_str += "s_sh" + code;
			} else {
				logger::code_error(code);
				return false;
			}
		}
		return true;
	}
    inline bool get_plain_qry_str(string& qry_str)
    {
		qry_str += "http://hq.sinajs.cn/list=";
        for (auto code : code_vec) {
            qry_str += code + ",";
        }
        qry_str = qry_str.substr(0, qry_str.size() - 1);
        return true;
    }
    inline bool get_code_jp_name_qry_str(std::string& qry_str)
    {
		qry_str = "http://suggest3.sinajs.cn/suggest/type=11&key=";
        std::string code = code_vec[code_jp_name_idx];
        if (code[0] == '6') {
            qry_str += "sh" + code;
        }
        else {
            qry_str += "sz" + code;
        }
        return true;
    }
	static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
		std::string* str = (std::string *) userdata;
		if (str == NULL || ptr == NULL) {
			logger::log_error("Error while crawl data");
		}
		size_t len = size * nmemb;

		str->append(ptr, len);
		return len;
	}
	bool crawler_content(std::string& user_data, gen_qry_str_func qry_str_func) {
		CURL *curl = curl_easy_init();
		string qry_str;
		if (!qry_str_func(qry_str)) { return false; }
		if (curl) {
			CURLcode res;
			curl_easy_setopt(curl, CURLOPT_URL, qry_str.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &user_data);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
		return true;
	}

private:
	cc_vec_string code_vec;
    uint32_t code_jp_name_idx;
};

}

#endif
