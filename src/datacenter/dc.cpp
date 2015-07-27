/**
*Author: Steve Zhong
*Creation Date: 2015年07月21日 星期二 20时44分46秒
*Last Modified: 2015年07月27日 星期一 20时18分17秒
*Purpose: 获取数据
**/

#include "dc_service.h"

int main(int argc, char *argv[])
{
    dc::dc_service *service = new dc::dc_service();
    service->configure();
    service->run();
    return 0;
}
