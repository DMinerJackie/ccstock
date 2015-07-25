/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 21时06分10秒
*Last Modified: 2015年07月25日 星期六 11时38分28秒
*Purpose:
**/

#ifndef DC_SERVICE_H_
#define DC_SERVICE_H_

#include <gateway/yahoo_history_crawler.h>
#include <common/configurator.h>
#include <common/logger.h>

#include "client/history_client.h"
#include "db/history_db.h"

namespace dc {

using history_crawler           = gateway::yahoo_history_crawler;
using history_db                = db::history_db;
using history_client_t          = history_client<history_crawler, history_db>;
using configurator              = common::configurator;
using logger                    = common::logger;

struct dc_io_data {
    ev_io watcher;
    configurator *config;
    history_client_t * history_client;
};

class dc_service {
public:
    dc_service():
        config(new configurator())
    {
        // 监听控制台输入
        ev_loop = EV_DEFAULT;
        io_watcher.history_client = &history_client_;
        ev_io_init(&io_watcher.watcher, read_cb, 0, EV_READ);
        ev_io_start(ev_loop, &io_watcher.watcher);
    }
    ~dc_service()
    {
        delete config;
    }
    // 配置程序信息和客户端
    void configure();
    // 开始事件循环
    void run();
private:
    // 控制台输入回调函数
    static void read_cb(EV_P_ ev_io *w, int revents);
private:
    configurator* config;
    // 历史数据客户端
    history_client_t history_client_;
private:
    struct ev_loop* ev_loop;
    dc_io_data io_watcher;
};

}

#endif
