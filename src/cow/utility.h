/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 10时36分22秒
*Purpose:
**/

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace cow {

class utility {
public:
	static bool split(const std::string& str, char delim, std::vector<std::string>& str_vec, bool delim_end = false)
	{
		size_t ps = 0, pe = 0;
		while ((pe = str.find_first_of(delim, ps)) != std::string::npos) {
			str_vec.push_back(str.substr(ps, pe - ps));
			ps = pe + 1;
		}
		if (!delim_end) {
			str_vec.push_back(str.substr(ps));
		}
		return true;
	}

	static bool subsplit(const std::string& str, char delim, std::string& substr) 
	{
		size_t ps = str.find_first_of(delim);
		if (ps++ == std::string::npos) return false;

		size_t pe = str.find_first_of(delim, ps + 1);
		if (pe == std::string::npos) return false;

		substr = str.substr(ps, pe - ps);

		return true;
	}
	
	static bool delete_char(std::string&str, char ch)
	{
		str.erase(std::remove(str.begin(), str.end(), ch), str.end());
		return true;
	}
	
	template <typename T>
	static std::string to_string(const T& param) { 
		return std::to_string(param);
	}
	
	template <typename T>
	static std::string to_string_pcs(const T& param, const size_t n = 6) 
	{
		std::ostringstream out;
		out << std::fixed << std::setprecision(n) << param;
		return out.str();
	}
};
}

#endif
