/**
*Author: Steve Zhong
*Creation Date: 2015年07月06日 星期一 20时14分24秒
*Last Modified: 2015年07月13日 星期一 00时19分56秒
*Purpose: 爬虫基类
**/

#ifndef COMMON_CRAWLER_H_
#define COMMON_CRAWLER_H_

#include <string>

#include <curl/curl.h>

#include <common/logger.h>
#include <common/common_defs.h>

namespace gateway {

class common_crawler {
protected:
    using logger = common::logger;
public:
    common_crawler() {}
    // 获取数据
    bool crawler_content(std::string& user_data, gen_qry_str_func qry_str_func) {
        CURL *curl = curl_easy_init();
        std::string qry_str;
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
protected:
    // 获取URL地址，须有子类实现
    virtual bool get_qry_str(std::string&) = 0;
    // 设置URL参数
    bool set_code_vec(const cc_vec_string& code_vec_)
    {
        this->code_vec = code_vec_;
        return true;
    }
    // 爬虫回调函数
    static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
        std::string* str = (std::string *) userdata;
        if (str == NULL || ptr == NULL) {
            logger::log_error("Error while crawl data");
        }
        size_t len = size * nmemb;

        str->append(ptr, len);
        return len;
    }
public:
    // 参数列表
    cc_vec_string code_vec;
};

}

#endif
