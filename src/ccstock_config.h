/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月06日 星期一 21时49分17秒
*Purpose:
**/

#ifndef CCSTOCK_CONFIG_H_
#define CCSTOCK_CONFIG_H_

#include "gateway/market_data_crawler.h"
#include "simulator/market_data_client.h"
#include "simulator/instrument/code_db.h"
#include "common/configurator.h"

using code_db       = simulator::code_db;
using md_crawler    = gateway::market_data_crawler;
using md_client     = simulator::market_data_client<code_db, md_crawler>;

using configurator  = common::configurator;

#endif
