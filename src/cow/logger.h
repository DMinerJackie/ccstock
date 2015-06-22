/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 11时54分20秒
*Purpose:
**/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>

using namespace std;

namespace cow {

class logger {
public:
	static void log_error(const string& error_info) { cerr << error_info << endl; }		
	static void log_debug(const string& debug_info) { cout << debug_info << endl; }
	
	static void code_error(const string& code) { cerr << "股票代码[" << code << "]不存在，请检查！" << endl; }
};

}

#endif
