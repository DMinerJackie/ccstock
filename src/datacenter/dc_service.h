/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 21时06分10秒
*Last Modified: 2015年07月24日 星期五 21时12分09秒
*Purpose:
**/

#ifndef DC_SERVICE_H_
#define DC_SERVICE_H_

#include <gateway/yahoo_history_crawler.h>
#include <simulator/history_client.h>
#include <common/configurator.h>
#include <common/logger.h>

using history_crawler           = gateway::yahoo_history_crawler;
using history_client_t          = simulator::history_client<history_crawler>;

using configurator              = common::configurator;
using logger                    = common::logger;

namespace dc {

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
    void configure();
    void run();
private:
    static void read_cb(EV_P_ ev_io *w, int revents);
private:
    configurator* config;
    history_client_t history_client_;
private:
    struct ev_loop* ev_loop;
    dc_io_data io_watcher;
};

}

#endif
