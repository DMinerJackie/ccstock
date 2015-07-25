/**
*Author: Steve Zhong
*Creation Date: 2015年07月11日 星期六 20时37分16秒
*Last Modified: 2015年07月11日 星期六 20时41分13秒
*Purpose:
**/
#ifndef SYSTIME_H
#define SYSTIME_H

#include <ctime>

namespace simulator {
namespace ui {

class systime {
public:
    static char* get_curr_time()
    {
        time_t t;
        time(&t);
        return ctime(&t);
    }
};

}
}

#endif

