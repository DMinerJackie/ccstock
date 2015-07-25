/**
*Author: Steve Zhong
*Creation Date: 2015年07月19日 星期日 16时06分06秒
*Last Modified: 2015年07月25日 星期六 12时33分06秒
*Purpose:
**/

#ifndef HISTORY_CLIENT_H_
#define HISTORY_CLIENT_H_

#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint> // int32_t, uint32_t

#include <common/file/file_handler.h>
#include <common/common_defs.h>
#include <common/io_aux.h>

namespace dc {

template <typename yahoo_crawler, typename history_db>
class history_client {
public:
    using self_type     = history_client<yahoo_crawler, history_db>;
    using file_handler  = common::file_handler;
public:
    void configure(const std::string& code_path_,
            const std::string& yahoo_data_path_,
            const std::string& yahoo_db_path_)
    {
        code_path = code_path_;
        yahoo_data_path = yahoo_data_path_;
        yahoo_db_path = yahoo_db_path_;

        // 读取所有股票代码
        file_handler::read_code(code_vec, code_path, "all");

        // 配置内存数据库
        history_db_.configure(code_vec, yahoo_db_path);

        common::io_aux::create_folder(yahoo_data_path.c_str());
    }
    void get_history_all()
    {
        cc_vec_string fname_vec;
        for (size_t idx = 0; idx < code_vec.size(); ++idx) {
            fname_vec.push_back(yahoo_data_path + code_vec[idx] + "_all.csv");
        }
        crawler.get_history_all(code_vec, fname_vec);
    }
private:
    std::string code_path;
    std::string yahoo_data_path;
    std::string yahoo_db_path;

    cc_vec_string code_vec;
private:
    yahoo_crawler crawler;
    history_db history_db_;
};

}
#endif
