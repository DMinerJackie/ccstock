/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 10时39分30秒
*Purpose:
**/

#ifndef IO_AUX_H
#define IO_AUX_H

#include <string>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <initializer_list>

using namespace std;

namespace tiger {

class io_aux {
public:
	static bool sd_pair(const char* midctl, const char* endctl, int count, ...)
	{
		std::va_list args;
		va_start(args, count);
		while (count--) {
			cout << va_arg(args, char*) << ": " 
				<< fixed << setprecision(2)
				<< va_arg(args, double) << midctl;
		}
		va_end(args);

		std::cout << endctl;
		return true;
	}

	template <typename T>
	static bool list_out(const char* midctl, const char* endctl, std::initializer_list<T> list)
	{
		int i = 0;
		for (auto elem : list) {
			cout << (i++ ? midctl : "") << elem;
		}
		cout << endctl;
	}

	template <typename Name, typename Value>
	static bool list_pair_out(const char* midctl, const char* endctl, 
			initializer_list<Name> nlist,
			initializer_list<Value> vlist)
	{
		const Name* nit = begin(nlist);
		const Value* vit = begin(vlist);
		while (nit != end(nlist)) {
			cout << (nit == begin(nlist) ? "" : midctl) << 
				*nit << ": " << fixed << setprecision(2) << *vit;
			++nit, ++vit;
		}
		cout << endctl;

		return true;
	}

	static void sd_pair_one(const string& name, const double& val) { printf("%s: %.2lf\t", name.c_str(), val); }	

	template <typename T, typename S>
	static void sd_pair_t(T& first, T& second, bool end = false) 
	{ 
		std::cout << first << "\t" << second; 
		if (end) { std::cout << endl; }
	}

	static void cmd2f(const double& val) { printf("%.2lf", val); }
	static void divider() { printf("----------------------------------------------------------------------\n"); };

};

}

#endif
