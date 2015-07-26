#include <common/system/cctime.h>

#include <string>
#include <iostream>

using namespace std;

void test_systime()
{
    cout << common::timeutility::get_curr_date() << endl;
    cout << common::timeutility::get_curr_date_time() << endl;
    common::timeutility cctime("2015-02-28");
    cout << cctime.add_day(1) << endl;
    cout << cctime.add_day(2) << endl;
    cout << cctime.add_day(3) << endl;
}

int test_common() 
{
    test_systime();
    return 0;
}
