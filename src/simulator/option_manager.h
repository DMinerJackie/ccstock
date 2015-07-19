/**
*Author: Steve Zhong
*Creation Date: 2015年07月04日 星期六 23时57分03秒
*Last Modified: 2015年07月13日 星期一 00时26分31秒
*Purpose: 自选股管理器
**/

#ifndef OPTION_MANAGER_H
#define OPTION_MANAGER_H

#include <string>
#include <memory>

#include <common/common_defs.h>
#include <common/utility.h>

#include "file_handler.h"

namespace simulator {

template <typename code_db, typename md_crawler, typename displayer>
class option_manager {
    using utility = common::utility;
    using self_type = option_manager<code_db, md_crawler, displayer>;
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
        if (db->from_jp_to_code(code_vec)) {
            file_handler::add_option(code_vec, option_path);
        }
    }
    void del_option(const std::string& options)
    {
        cc_vec_string code_vec;
        utility::split(options, ',', code_vec);
        if (db->from_jp_to_code(code_vec)) {
            file_handler::del_option(code_vec, option_path);
        }
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
        displayer_.print_options(stock_vec);
    }
private:
    std::string option_path;
    shared_ptr<code_db> db;
    md_crawler crawler_;
    displayer displayer_;
};

}

#endif
