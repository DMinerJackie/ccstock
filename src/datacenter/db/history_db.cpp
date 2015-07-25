/**
*Author: Steve Zhong
*Creation Date: 2015年07月25日 星期六 11时14分13秒
*Last Modified: 2015年07月25日 星期六 12时30分31秒
*Purpose:
**/

#include <tools/history.h>

#include "history_db.h"

namespace dc {
namespace db {

// 静态类成员定义
std::unordered_map<std::string, std::string> history_db::yahoo_end;

bool history_db::configure(const cc_vec_string& code_vec,
        const string& yahoo_db_path)
{
    kv_db_.configure(yahoo_db_path);
    kv_db_.open();
    std::string end_date;
    for (auto code : code_vec) {
        if (kv_db_.select(code, end_date).ok()) {
            yahoo_end[code] = end_date;
        }
        else {
            yahoo_end[code] = BEGIN_DATE;
        }
    }
    kv_db_.close();
    return true;
}

bool history_db::yahoo_check_empty(const std::string& code) const 
{
    return yahoo_end.find(code) == yahoo_end.end();
}

void history_db::yahoo_update_end(const std::string& code, const std::string& date)
{
    yahoo_end[code] = date;
}

std::string history_db::yahoo_get_end(const std::string& code)
{
    return yahoo_end[code];
}

cc_vec_string history_db::yahoo_get_end_multi(const cc_vec_string& code_vec)
{
    cc_vec_string date_vec;
    for (auto code : code_vec) {
        date_vec.push_back(yahoo_end[code]);
    }
    return date_vec;
}

}
}