#include "../common/system/datetime.h"

#include <string>
#include <iostream>

using namespace std;

void test_systime()
{
    common::datetime dt;
    cout << dt.get_curr_time() << endl;
}

int test_common() 
{
    test_systime();
    return 0;
}
