/**
*Author: Steve Zhong
*Creation Date: 2015年07月04日 星期六 22时34分11秒
*Last Modified: 2015年07月05日 星期日 00时54分12秒
*Purpose:
**/
#ifndef CODE_DB_H_
#define CODE_DB_H_

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

#include "stock.h"
#include "simulator/file_handler.h"

namespace simulator {

// 内存数据库，方便快速查找验证
class code_db {
public:
    //
    bool configure(const std::string& code_path)
    {
        std::vector<stock_basic> code_info;
        file_handler::read_code_jp_name(code_info, code_path);
        for (auto info : code_info) {
            code_set.insert(info.code);
            jp_code_map[info.jp] = inner_stock_info(info.code, info.name, info.market_code);
        }
        return true;
    }
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
    std::set<std::string> code_set;
    std::unordered_map<std::string, inner_stock_info> jp_code_map;
};

}

#endif
