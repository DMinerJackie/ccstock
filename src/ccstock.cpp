/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年08月01日 星期六 11时21分21秒
*Purpose:
**/

#include <iostream>
#include <string>
#include <memory> // shared_ptr

#include <common/configurator.h>

#include "mdsim.h"

int main(int argc, char* argv[]) {
    using configurator = common::configurator;
    // 设置程序选项
    shared_ptr<configurator> configurator_ (new configurator());
    configurator_->add_option("code,a", "查看个股信息，代码用','分开", std::string());
    configurator_->add_option("config,c", "设置配置文件", std::string());
    configurator_->add_option("data,d", "批量显示行情", std::string());
    configurator_->add_plain_option("market,m", "显示大盘行情");
    configurator_->add_option("top,t", "筛选前几个信息", int());
    configurator_->add_option("order,o", "排序规则", std::string());
    configurator_->add_option("add-option", "追加自选股,股票代码用','分开", std::string());
    configurator_->add_option("del-option", "删除自选股,股票代码用','分开", std::string());
    configurator_->add_plain_option("show-option,O", "查看自选股");
    configurator_->add_plain_option("init", "初始化股票代码");
    configurator_->parse_command_line(argc, argv);
    // 显示帮助信息
    if (configurator_->is_option_set("help")) {
        configurator_->show_options();
        return 0;
    }
    mdsim_service service(configurator_);
    // 查看个股行情
    if (configurator_->is_option_set("code")) {
        service.show_stock_data(configurator_->get_string_option("code"));
    }
    // 查看大盘行情
    if (configurator_->is_option_set("market")) {
        service.show_market();
    }
    // 增加自选股
    if (configurator_->is_option_set("add-option")) {
        service.add_option(configurator_->get_string_option("add-option"));
    }
    // 删除自选股
    if (configurator_->is_option_set("del-option")) {
        service.del_option(configurator_->get_string_option("del-option"));
    }
    // 显示自选股行情 
    if (configurator_->is_option_set("show-option")) {
        service.show_option();
    }
    // 初始化股票代码
    if (configurator_->is_option_set("init")) {
        service.code_initialize();
    }
    // 批量显示股票行情 
    if (configurator_->is_option_set("data")) {
        int32_t top_num = -1;
        std::string order("");
        // 筛选top n
        if (configurator_->is_option_set("top")) {
            top_num = configurator_->get_int_option("top");
        }
        // 排序规则
        if (configurator_->is_option_set("order")) {
            order= configurator_->get_string_option("order");
        }
        service.show_market_data(configurator_->get_string_option("data"),
                configurator_->get_value("stock.market_data.request_speed", uint32_t()),
                top_num,
                order);
    }

    return 0;
}
