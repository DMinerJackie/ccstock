/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月27日 星期六 21时00分46秒
*Purpose:
**/

#ifndef CRAWLER_H_
#define CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>

#include <curl/curl.h>

#include "stock.h"
#include "sina_decoder.h"

#include "../common/logger.h"
#include "../common/utility.h"
#include "../simulator/common_defs.h"

namespace gateway {

class crawler {
public:
    using self_type = crawler;
	using decoder	= sina_decoder;
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

    bool list_code_name(std::vector<std::string>&code_vec, std::vector<stock>& stock_vec)
    {
        size_t i = 0;
        for (auto code : code_vec) {
            set_code_vec({code}); 
            run_code(stock_vec);
            if (i > 0 && i % 200 == 0) {
                logger::log_debug_variadic("process ", code_vec[i - 200], "-", code_vec[i], " finished!"); 
            }
            ++i;
        }
        return true;
    }
    bool list_stock(const std::vector<std::string>& code_vec, std::vector<stock>& stock_vec)
    {
        set_code_vec(code_vec); 
        run_code(stock_vec);
        return true;
    }
    bool show_market(const std::vector<std::string>& code_vec, std::vector<market>& market_vec)
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
    bool set_code_vec(const std::vector<std::string>& code_vec_)
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
    inline bool get_plain_qry_str(string& qry_str)
    {
		qry_str += "http://hq.sinajs.cn/list=";
        for (auto code : code_vec) {
            qry_str += code + ",";
        }
        qry_str = qry_str.substr(0, qry_str.size() - 1);
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
	std::vector<std::string> code_vec;	
};

}

#endif
