/**
*Author: Steve Zhong
*Creation Date: 2015年07月19日 星期日 16时06分06秒
*Last Modified: 2015年07月19日 星期日 16时49分10秒
*Purpose:
**/

#ifndef HISTORY_CLIENT_H_
#define HISTORY_CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint> // int32_t, uint32_t

#include <simulator/file_handler.h>
#include <common/common_defs.h>
#include <common/io_aux.h>

namespace simulator {

template <typename history_crawler>
class history_client {
public:
    using self_type     = history_client<history_crawler>;
    using file_handler  = simulator::file_handler;
public:
    void configure(const std::string& code_path_, const std::string& data_path_)
    {
        code_path = code_path_;
        data_path = data_path_;
        common::io_aux::create_folder(data_path.c_str());
    }
    void get_history_all()
    {
        cc_vec_string code_vec;
        cc_vec_string fname_vec;
        file_handler::read_code(code_vec, code_path, "all");
        for (size_t idx = 0; idx < code_vec.size(); ++idx) {
            fname_vec.push_back(data_path + code_vec[idx] + "_all.csv");
        }
        crawler.get_history_all(code_vec, fname_vec);
    }
private:
    std::string code_path;
    std::string data_path;
    history_crawler crawler;
};

}
#endif
