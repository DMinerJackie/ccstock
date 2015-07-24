/**
*Author: Steve Zhong
*Creation Date: 2015年07月06日 星期一 20时14分24秒
*Last Modified: 2015年07月20日 星期一 00时01分01秒
*Purpose: 爬虫基类
**/

#ifndef COMMON_CRAWLER_H_
#define COMMON_CRAWLER_H_

#include <string>
#include <cstdio>

#include <curl/curl.h>

#include <common/logger.h>
#include <common/common_defs.h>

namespace gateway {

class common_crawler {
protected:
    using logger = common::logger;
public:
    common_crawler()
    {
        curl = curl_easy_init();
    }
    ~common_crawler() {
        curl_easy_cleanup(curl);
    }
    // 获取数据
    bool crawler_content(std::string& user_data, gen_qry_str_func qry_str_func) {
        if (!qry_str_func()) { return false; }
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, qry_str.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &user_data);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
            curl_easy_perform(curl);
            return true;
        }
        return false;
    }
    // 下载文件
    bool download(const char *out_file_name, gen_qry_str_func qry_str_func)
    {
        if (!qry_str_func()) { return false; }
        if (curl) {
            FILE *fp = fopen(out_file_name, "wb");
            curl_easy_setopt(curl, CURLOPT_URL, qry_str.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_perform(curl);
            fclose(fp);
            return true;
        }
        return false;
    }
protected:
    // 获取URL地址，须有子类实现
    virtual bool get_qry_str() = 0;
    // 设置URL参数
    bool set_code_vec(const cc_vec_string& code_vec_)
    {
        this->code_vec = code_vec_;
        return true;
    }
    // 写文本回调函数
    static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
        std::string* str = (std::string *) userdata;
        if (str == NULL || ptr == NULL) {
            logger::log_error("Error while crawl data");
        }
        size_t len = size * nmemb;

        str->append(ptr, len);
        return len;
    }
    // 下载文件回调函数
    static size_t download_cb(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        size_t write_len = fwrite(ptr, size, nmemb, stream);
        return write_len;
    }
public:
    // 参数列表
    cc_vec_string code_vec;
    // libcurl句柄
    CURL *curl;
    // 查询字符串
    std::string qry_str;
};

}

#endif
