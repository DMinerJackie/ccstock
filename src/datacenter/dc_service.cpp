/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 23时08分02秒
*Last Modified: 2015年07月23日 星期四 23时37分45秒
*Purpose:
**/

#include "dc_service.h"

namespace dc {

void dc_service::configure()
{
    config->add_plain_option("history,H", "获取历史数据");
    // config->parse_command_line(argc, argv);
    config->parse_xml_file("dc.xml");
    // 传递配置信息
    io_watcher.config = config;
}
void dc_service::run()
{
    logger::log_info_inline("请输入相关命令> ");
    ev_run(ev_loop, 0); 
}
void dc_service::read_cb(EV_P_ ev_io *w, int revents)
{
    std::string command;
    std::cin >> command;
    if (command == "history" || command == "H")
    {

    }
    logger::log_info_inline("请输入相关命令> ");
}

}
