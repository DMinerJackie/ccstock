/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 10时36分41秒
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

#include "../cow/logger.h"
#include "../cow/utility.h"

namespace muridae {

class crawler {
public:
	using decoder	= sina_decoder;
	using logger	= cow::logger;
	using utility	= cow::utility;

public:
	crawler() {}
	
	bool config(int argc, char* argv[]) {
		if (argc < 1) return false;
		if (argc == 3 && strcmp(argv[1], "-c") == 0) {
			utility::split(std::string(argv[2]), ',', code_vec);						
		}
		return true;
	}

	bool run(std::vector<stock>& stock_vec) {
		CURL *curl = curl_easy_init();
		string qry_str;
		if (! get_qry_str(qry_str)) { return false; }
		if (curl) {
			std::string stock_data;
			CURLcode res;
			curl_easy_setopt(curl, CURLOPT_URL, qry_str.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &stock_data);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
	
			if(!decoder::decode(stock_data, stock_vec)) { return false; }
		}	
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
	
	static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
		std::string* str = (std::string *) userdata;
		if (str == NULL || ptr == NULL) {
			logger::log_error("Error while crawl data");
		}
		size_t len = size * nmemb;

		str->append(ptr, len);
		return 0;
	}

private:
	std::vector<std::string> code_vec;	
};

}

#endif
