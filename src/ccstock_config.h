/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月05日 星期日 00时49分59秒
*Purpose:
**/

#ifndef CCSTOCK_CONFIG_H_
#define CCSTOCK_CONFIG_H_

#include "simulator/market_data_client.h"
#include "simulator/instrument/code_db.h"
#include "common/configurator.h"

using code_db       = simulator::code_db;
using md_client     = simulator::market_data_client<code_db>;

using configurator  = common::configurator;

#endif
