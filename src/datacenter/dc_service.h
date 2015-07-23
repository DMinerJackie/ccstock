/**
*Author: Steve Zhong
*Creation Date: 2015年07月23日 星期四 21时06分10秒
*Last Modified: 2015年07月24日 星期五 00时36分45秒
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

struct dc_io_watcher {
    ev_io watcher;
    configurator *config;
};

class dc_service {
public:
    dc_service():
        config(new configurator())
    {
        ev_loop = EV_DEFAULT;
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
    struct ev_loop* ev_loop;
    dc_io_watcher io_watcher;
};

}

#endif
