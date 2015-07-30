/**
*Author: Steve Zhong
*Creation Date: 2015年07月06日 星期一 23时54分32秒
*Last Modified: 2015年07月30日 星期四 22时01分28秒
*Purpose:
**/
#include <vector>
#include <string>

#include <common/file/file_handler.h>

using namespace std;

void test_file_list()
{
    vector<string> file_list;
    common::file_handler::get_file_list(file_list, "./data/history/yahoo/");
    for (auto name : file_list)
        cout << name << endl;
}

void test_filehandler()
{
    test_file_list();
}
