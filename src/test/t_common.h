#include <common/system/datetime.h>

#include <string>
#include <iostream>

using namespace std;

void test_systime()
{
    cout << common::get_curr_time() << endl;
}

int test_common() 
{
    test_systime();
    return 0;
}
