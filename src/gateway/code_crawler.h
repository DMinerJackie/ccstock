/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月09日 星期四 17时57分56秒
*Purpose:
**/
#ifndef CODE_CRAWLER_H_
#define CODE_CRAWLER_H_

#include <cstring>
#include <string>
#include <vector>

#include "sina_decoder.h"

#include "common/utility.h"
#include "common/common_defs.h"

#include "common_crawler.h"

namespace gateway {

class code_crawler : public common_crawler {
public:
    using self_type = code_crawler;
    using decoder   = sina_decoder;
    using stock_basic = simulator::stock_basic;
    using utility   = common::utility;
public:
    bool gen_code(cc_vec_string& init_code, cc_vec_string& final_code)
    {
        set_code_vec(init_code);
        std::string code_data;
        crawler_content(code_data, std::bind(&self_type::get_qry_str,
                this,
                std::placeholders::_1));
        decoder::decode_code(code_data, final_code);
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
private:
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
private:
    uint32_t code_jp_name_idx;
};

}

#endif
