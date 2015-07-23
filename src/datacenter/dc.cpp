/**
*Author: Steve Zhong
*Creation Date: 2015年07月21日 星期二 20时44分46秒
*Last Modified: 2015年07月23日 星期四 23时28分10秒
*Purpose: 获取数据
**/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>

#include <ev.h>

#include "dc_service.h"

int main(int argc, char *argv[])
{
    dc::dc_service *service = new dc::dc_service();
    service->configure();
    service->run();
    return 0;
}
