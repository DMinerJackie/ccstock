#include <common/system/cctime.h>

#include <string>
#include <iostream>

using namespace std;

void test_systime()
{
    using cctime = common::timewrapper;
    cout << cctime::get_curr_date() << endl;
    cout << cctime::get_curr_date() << endl;
    cout << cctime::get_curr_date_time() << endl;
    cout << cctime::get_curr_date_time() << endl;

    tm* timeinfo = cctime::get_tm_from_string("2015-02-28");
    cout << cctime::get_string_from_tm(timeinfo) << endl;
    cctime::add_day(timeinfo, 1);
    cout << cctime::get_string_from_tm(timeinfo) << endl;
    cctime::add_day(timeinfo, 1);
    cout << cctime::get_string_from_tm(timeinfo) << endl;
    cctime::add_day(timeinfo, 1);
    cout << cctime::get_string_from_tm(timeinfo) << endl;

    cctime timeobj("2015-02-28");
    timeobj.add_day(1);
    cout << timeobj.date_to_string() << endl;
}

int test_common() 
{
    test_systime();
    return 0;
}
