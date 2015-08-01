/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 23时08分02秒
*Last Modified: 2015年08月01日 星期六 10时32分27秒
*Purpose:
**/

#include <string> // getline();

#include "dc_service.h"

#include <common/logger.h>
#include <common/configurator.h>

namespace dc {

void dc_service::configure()
{
    // 添加程序选项
    config->add_plain_option("history,H", "获取历史数据");
    config->add_plain_option("init-endtime", "手动初始化时间");
    config->add_plain_option("quit,q", "退出程序");
    config->parse_xml_file("dc.xml");
    // 传递配置信息
    io_watcher.config = config;
    // 股票代码位置
    std::string code_path = config->get_value("data.code_path", std::string());
    // 设置历史数据存放位置
    history_client_.configure(code_path,
            config->get_value("data.locate.file.yahoo", std::string()),
            config->get_value("data.locate.db.yahoo", std::string()),
            config->get_value("data.api.history.yahoo", std::string()));
}

void dc_service::run()
{
    common::logger::log_info_inline("请输入程序选项及参数> ");
    ev_run(ev_loop, 0); 
}

// 标准输入回调函数
void dc_service::read_cb(EV_P_ ev_io *w, int revents)
{
    dc_io_data* io_data = (dc_io_data*) w;
    common::configurator *config = io_data->config;
    auto history_client = io_data->history_client;
    // 输入程序相关选项
    std::string options;
    std::getline(cin, options);
    // 解析字符串程序选项
    if (config->parse_string_options(options)) {
        // 获取历史数据
        if (config->is_option_set("history")) {
            history_client->get_history_inc();
        }
        // 手动初始化时间
        else if (config->is_option_set("init-endtime")) {
            history_client->init_endtime();
        }
        // 退出
        else if (config->is_option_set("quit")) {
            ev_break(EV_A_ EVBREAK_ALL); 
            return;
        }
        // 帮助命令
        else if (config->is_option_set("help")) {
            config->show_options();
        }
        // 程序选项不存在
        else {
            common::logger::option_error(options);
        }
        // 删除所有选项
        config->delete_option_value();
    }
    common::logger::log_info_inline("请输入程序选项及参数> ");
}

}
