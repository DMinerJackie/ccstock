/**
*Author: Steve Zhong
*Creation Date: 2015年07月25日 星期六 10时58分00秒
*Last Modified: 2015年07月25日 星期六 12时02分59秒
*Purpose: leveldb持久化存储
**/

#ifndef HISTORY_DB_H_
#define HISTORY_DB_H_

#include <vector>
#include <string>
#include <unordered_map>

#include <common/common_defs.h>

#include "persistent_kv_db.h"

namespace dc {
namespace db {

class history_db {
public:
    bool configure(const cc_vec_string& code_vec, const::string& yahoo_db_path);
public:
    // 判断某只股票是否未获取历史数据
    bool yahoo_check_empty(const std::string& code) const;
    // 更新已获得的历史数据
    void yahoo_update_end(const std::string& code, const std::string& date);
    // 获取某只股票历史数据end时间
    std::string yahoo_get_end(const std::string& code);
    // 批量获取股票历史数据end时间
    cc_vec_string yahoo_get_end_multi(const cc_vec_string& code_vec);
private:
    std::string code_path;
    // yahoo已获得的历史数据结构
    static std::unordered_map<std::string, std::string> yahoo_end;
private:
    kv_db kv_db_;

}; 

}
}

#endif
