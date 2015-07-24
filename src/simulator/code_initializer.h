/**
*Author: Steve Zhong
*Creation Date: 2015年07月06日 星期一 23时54分32秒
*Last Modified: 2015年07月24日 星期五 21时26分52秒
*Purpose:
**/
#ifndef CODE_INITIALIZER_H
#define CODE_INITIALIZER_H

#include <memory>

#include <common/configurator.h>
#include <common/io_aux.h>
#include <common/common_defs.h>
#include <common/logger.h>
#include <common/utility.h>

#include "file_handler.h"

namespace simulator {

template <typename crawler>
class code_initializer {
public:
    using configurator  = common::configurator;
    using io_aux        = common::io_aux;
    using logger        = common::logger;
    using utility       = common::utility;
public:
    code_initializer(const shared_ptr<configurator>& config_):
        config(config_)
    {
    }
    bool run()
    {
        code_path = config->get_value("stock.market_data.code.code_path", std::string());
        if (!check_prerequisite()) {
            logger::log_info("系统首次运行初始化...");
            manual_initialize();
        }
        return true;
    }
    void manual_initialize()
    {
        if (count++ == 0) {
            gen_code();
            gen_code_jp_name();
        }
    }

private:
    // 检查数据文件是否存在
    bool check_prerequisite()
    {
        return io_aux::check_files_exist({code_path + "all_code.ds",
                code_path + "cyb_code.ds",
                code_path + "zxb_code.ds",
                code_path + "sz_code.ds",
                code_path + "sh_code.ds",
                code_path + "code_jp_name.ds"});
    }
    void gen_code()
    {
        cc_vec_string sh_code_range, sz_code_range, zxb_code_range, cyb_code_range, all_code_range;
        cc_vec_string sh_code, sz_code, zxb_code, cyb_code, all_code;

        gen_code_cyb(cyb_code_range);
        gen_code_bk(cyb_code, cyb_code_range, "cyb");
        logger::log_info("创业板股票代码生成完毕！");

        gen_code_zxb(zxb_code_range);
        gen_code_bk(zxb_code, zxb_code_range, "zxb");
        logger::log_info("中小板股票代码生成完毕！");

        gen_code_sz(sz_code_range);
        gen_code_bk(sz_code, sz_code_range, "sz");
        sz_code.insert(sz_code.end(), zxb_code.begin(), zxb_code.end());
        sz_code.insert(sz_code.end(), cyb_code.begin(), cyb_code.end());
        logger::log_info("深圳市场股票代码生成完毕！");

        gen_code_sh(sh_code_range);
        gen_code_bk(sh_code, sh_code_range, "sh");
        logger::log_info("上海市场股票代码生成完毕！");

        all_code.insert(all_code.end(), sz_code.begin(), sz_code.end());
        all_code.insert(all_code.end(), sh_code.begin(), sh_code.end());
        file_handler::save_code(all_code, code_path, "all_code.ds");
        logger::log_info("A股市场股票代码生成完毕！");
    }
    void gen_code_jp_name()
    {
        cc_vec_string code_vec;
        file_handler::read_code(code_vec, code_path, "all");
        std::vector<stock_basic> code_jp_name_vec;
        crawler_.gen_code_jp_name(code_vec, code_jp_name_vec);
        file_handler::save_code_jp_name(code_jp_name_vec, code_path);
    }
private:
    void gen_code_bk(cc_vec_string& final_code, cc_vec_string& init_code, const std::string& bk)
    {
        uint32_t idx = 0;
        uint32_t speed = config->get_value("stock.market_data.request_speed", uint32_t());
        cc_vec_string sub_code;
        while (utility::get_vec_by_step(init_code, idx, speed, sub_code)) {
            crawler_.gen_code(sub_code, final_code);
        }
        if (!sub_code.empty()) {
            crawler_.gen_code(sub_code, final_code);
        }
        file_handler::save_code(final_code, code_path, bk + "_code.ds");
    }
    void gen_code_sh(cc_vec_string& codes)
    {
        int beg = config->get_value("stock.market_data.code.sh.beg", int());
        int end = config->get_value("stock.market_data.code.sh.end", int());
        utility::gen_range_str(beg, end, codes);
    }
    void gen_code_sz(cc_vec_string& codes)
    {
        int beg = config->get_value("stock.market_data.code.sz.beg", int());
        int end = config->get_value("stock.market_data.code.sz.end", int());
        utility::gen_range_str(beg, end, codes);
        config->get_multi_value("stock.market_data.code.sz.specials", codes);
    }
    void gen_code_zxb(cc_vec_string& codes)
    {
        int beg = config->get_value("stock.market_data.code.sz.zxb.beg", int());
        int end = config->get_value("stock.market_data.code.sz.zxb.end", int());
        utility::gen_range_str(beg, end, codes);
    }
    void gen_code_cyb(cc_vec_string& codes)
    {
        int beg = config->get_value("stock.market_data.code.sz.cyb.beg", int());
        int end = config->get_value("stock.market_data.code.sz.cyb.end", int());
        utility::gen_range_str(beg, end, codes);
    }
private:
    crawler crawler_;
    std::string code_path; // 股票代码文件存放
    std::shared_ptr<configurator> config;
private:
    static int count;
};

template <typename crawler>
int code_initializer<crawler>::count = 0;

}
#endif
