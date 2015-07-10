/**
*Author: Steve Zhong
*Creation Date: 2015年07月04日 星期六 23时57分03秒
*Last Modified: 2015年07月09日 星期四 16时00分37秒
*Purpose: 自选股管理器
**/

#ifndef OPTION_MANAGER_H
#define OPTION_MANAGER_H

#include <string>
#include <memory>

#include "cmd_display.h"
#include "common/common_defs.h"
#include "common/utility.h"

#include "file_handler.h"

namespace simulator {

template <typename code_db, typename md_crawler>
class option_manager {
    using utility = common::utility;
    using displayer = cmd_display;
    using self_type = option_manager<code_db, md_crawler>;
public:
    void configure(const std::shared_ptr<code_db>& db_, const std::string& option_path_)
    {
        option_path = option_path_;
        db = db_;
    }
    void add_option(const std::string& options)
    {
        cc_vec_string code_vec;
        utility::split(options, ',', code_vec);
        file_handler::add_option(code_vec, option_path);
    }
    void del_option(const std::string& options)
    {
        cc_vec_string code_vec;
        utility::split(options, ',', code_vec);
        file_handler::del_option(code_vec, option_path);
    }
    void show_option()
    {
        cc_vec_string code_vec;
        file_handler::read_option(code_vec, option_path);

        crawler_.get_stock_data(code_vec,
                std::bind(&self_type::display_options,
                    this,
                    std::placeholders::_1),
                true);
        crawler_.run(); 
    }
private:
    void display_options(const std::vector<stock>& stock_vec)
    {
        displayer::stock_info_basic(stock_vec);
    }
private:
    std::string option_path;
    shared_ptr<code_db> db;
    md_crawler crawler_;
};

}

#endif
