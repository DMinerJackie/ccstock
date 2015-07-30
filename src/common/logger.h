/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月30日 星期四 21时39分53秒
*Purpose:
**/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

namespace common {

class logger {
public:
    static void log_error(const string& error_info) { cerr << error_info << endl; }
    static void log_debug(const string& debug_info) { cout << debug_info << endl; }
    template <typename T>
    static void log_info(const T& info) { cout << info << endl; }
    // 不加换行
    template <typename T>
    static void log_info_inline(const T& info) { cout << info << flush; }

    template <typename T>
    static void log_info_file(const vector<T>& vec)
    {
        ofstream ofs("log", ios::app);
        for (auto elem : vec) {
            ofs << elem << endl;
        }
        ofs.close();
    }

    template <typename T>
    static void log_debug_variadic(T& t)
    {
        cout << t << endl;
    }
    template <typename T, typename... Args>
    static void log_debug_variadic(T& t, Args... args)
    {
        cout << t;
        log_debug_variadic(args...);
    }

    static void code_error(const string& code) { cerr << "股票代码(简拼)[" << code << "]不存在，请检查！" << endl; }
    static void cmd_error(const string& code) { cerr << "未知命令[" << code << "]，请检查！" << endl; }
    static void dir_not_exist(const string& code) { cerr << "目录[" << code << "]不存在，请检查！" << endl; }
};

}

#endif
