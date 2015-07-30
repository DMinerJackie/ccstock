/**
*Author: Steve Zhong
*Creation Date: 2015年07月30日 星期四 19时27分12秒
*Last Modified: 2015年07月31日 星期五 01时01分19秒
*Purpose: yahoo历史数据处理 
**/

#ifndef YAHOO_HISTORY_HANDLER_H_
#define YAHOO_HISTORY_HANDLER_H_

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include <common/utility.h>
#include <common/file/file_handler.h>

#include <datacenter/instrument/yahoo_history_data.h>

namespace dc {

class yahoo_hist_handler {
    using yahoo_hist = dc::inst::yahoo_hist;
    using file_handler = common::file_handler;
public:
    void configure(const std::string& csv_path_)
    {
        csv_path = csv_path_;
    }
    // 解析csv文件，返回最新日期
    std::string parse_csv(const std::string& file_name, std::vector<yahoo_hist>& hist_data)
    {
        std::string temp, endtime("");
        ifstream ifs(file_name.c_str());
        getline(ifs, temp);
        if (temp.substr(0, 5) != "Date,") { return ""; }
        // 创建数据结构
        yahoo_hist hist;
        // 分析第一行
        getline(ifs, temp);
        // 解析成员值
        assign_member(hist, temp);
        hist_data.push_back(hist);
        endtime = hist.date;
        ifs.close();
        return endtime;
    }
    // 删除csv文件
    bool delete_csv_file(const std::string& file_name)
    {
        return file_handler::remove_file(file_name);
    }
private:
    void assign_member(yahoo_hist& hist, const std::string& data)
    {
        common::logger::log_info(data);
        std::vector<std::string> str_vec;
        common::utility::split(data, ',', str_vec);
        hist.date      = str_vec[0];
        hist.open      = std::stod(str_vec[1]);
        hist.high      = std::stod(str_vec[2]);
        hist.low       = std::stod(str_vec[3]);
        hist.close     = std::stod(str_vec[4]);
        hist.volumn    = std::stod(str_vec[5]);
        hist.adj_close = std::stod(str_vec[6]);
    }
private:
    std::string csv_path;
};

}

#endif
