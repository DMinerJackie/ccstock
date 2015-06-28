/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月24日 星期三 20时17分58秒
*Purpose:
**/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>

using namespace std;

namespace common {

class logger {
public:
	static void log_error(const string& error_info) { cerr << error_info << endl; }		
	static void log_debug(const string& debug_info) { cout << debug_info << endl; }
    template <typename T>
	static void log_info(const T& info) { cout << info << endl; }
	
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
	
	static void code_error(const string& code) { cerr << "股票代码[" << code << "]不存在，请检查！" << endl; }
};

}

#endif
