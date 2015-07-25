/**
*Author: Steve Zhong
*Creation Date: 2015年07月04日 星期六 22时34分11秒
*Last Modified: 2015年07月13日 星期一 00时22分30秒
*Purpose:
**/

#ifndef CODE_DB_H_
#define CODE_DB_H_

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <cctype>

#include <common/logger.h>
#include <common/file/file_handler.h>

#include "stock.h"

namespace simulator {

// 内存数据库，方便快速查找验证
class code_db {
    using logger       = common::logger;
    using file_handler = common::file_handler;
public:
    // 配置数据
    bool configure(const std::string& code_path);
    // 检查股票代码是否存在
    bool check_code_exist_multi(const std::vector<std::string>& code_vec);
    // 将简拼转换为代码
    bool from_jp_to_code(std::vector<std::string>& vec);
private:
    // 检查单个股票代码是否存在
    bool check_code_exist(const string& code);
private:
    // 股票信息内部类
    struct inner_stock_info {
        inner_stock_info() { }
        inner_stock_info(const std::string code_, const std::string name_, const std::string market_code_):
            code(code_),
            name(name_),
            market_code(market_code_)
        {
        }
        std::string code;           // 代码
        std::string name;           // 名称
        std::string market_code;    // 加上sh/sz的代码
    };
private:
    // 股票代码
    static std::set<std::string> code_set;
    // 股票简拼和相关信息映射
    static std::unordered_map<std::string, inner_stock_info> jp_code_map;
};

std::set<std::string> code_db::code_set;
std::unordered_map<std::string, code_db::inner_stock_info> code_db::jp_code_map;

bool code_db::configure(const std::string& code_path)
{
    std::vector<stock_basic> code_info;
    file_handler::read_code_jp_name(code_info, code_path);
    for (auto info : code_info) {
        code_set.insert(info.code);
        jp_code_map[info.jp] = inner_stock_info(info.code, info.name, info.market_code);
    }
    return true;
}

bool code_db::check_code_exist(const string& code)
{
    if (code_set.find(code) == code_set.end()) {
        logger::code_error(code);
        return false;
    }
    return true;
}

bool code_db::check_code_exist_multi(const std::vector<std::string>& code_vec)
{
    bool flag = true;
    for (auto code : code_vec) {
        if (code_set.find(code) == code_set.end()) {
            logger::code_error(code);
            flag = false;
        }
    }
    return flag;
}

bool code_db::from_jp_to_code(std::vector<std::string>& vec)
{
    bool flag = true;
    for (auto &elem : vec) {
        if (isdigit(elem[0])) {
            if (!check_code_exist(elem)) {
                flag = false;
            }
        }
        else {
            auto iter = jp_code_map.find(elem);
            if (iter != jp_code_map.end()) {
                elem = (iter->second).code;
            }
            else {
                logger::code_error(elem);
                flag = false;
            }
        }
    }
    return flag;
}

}

#endif
