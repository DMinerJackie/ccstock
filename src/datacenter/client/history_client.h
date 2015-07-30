/**
*Author: Steve Zhong
*Creation Date: 2015年07月19日 星期日 16时06分06秒
*Last Modified: 2015年07月31日 星期五 00时41分41秒
*Purpose:
**/

#ifndef HISTORY_CLIENT_H_
#define HISTORY_CLIENT_H_

#include <string>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint> // int32_t, uint32_t

#include <common/logger.h>
#include <common/common_defs.h>
#include <common/io_aux.h>
#include <common/system/cctime.h>
#include <common/file/file_handler.h>

#include <datacenter/tools/history.h>
#include <datacenter/tools/yahoo_history_handler.h>
#include <datacenter/db/persistent_kv_db.h>
#include <datacenter/instrument/yahoo_history_data.h>

namespace dc {

template <typename yahoo_crawler, typename history_db>
class history_client {
public:
    using self_type     = history_client<yahoo_crawler, history_db>;
    using file_handler  = common::file_handler;
    using yahoo_hist    = dc::inst::yahoo_hist;
public:
    void configure(const std::string& code_path_,
            const std::string& yahoo_data_path_,
            const std::string& yahoo_db_path_,
            const std::string& yahoo_api_)
    {
        code_path = code_path_;
        // yahoo历史数据(csv文件)存放地址
        yahoo_data_path = yahoo_data_path_;
        // 读取所有股票代码
        file_handler::read_code(code_vec, code_path, "option");
        // 配置内存数据库
        history_db_.configure(yahoo_db_path_);
        // 配置LevelDB
        kv_db_.configure(yahoo_db_path_);
        // 配置crawler
        crawler.configure(yahoo_api_);
        // yahoo历史数据处理器
        hist_handler.configure(yahoo_data_path_);
        // 创建文件夹
        common::io_aux::create_folder(yahoo_data_path.c_str());
    }
    // 增量方式获得部分股票历史数据
    void get_history_inc();
    // 处理历史数据
    void handle_history(const std::vector<std::string>& fname_vec, std::vector<common::timewrapper>& endtime_vec);
    // 手动更新历史数据结束时间
    void init_endtime();
private:
    // 获取每只股票的最新历史数据的endtime
    void get_endtime(std::vector<common::timewrapper>& endtime_vec);
    // 更新每只股票的最新历史数据的endtime
    void update_endtime(std::vector<common::timewrapper>& endtime_vec);
private:
    std::string code_path;
    std::string yahoo_data_path;
    // 所有的股票代码
    cc_vec_string code_vec;
private:
    yahoo_crawler crawler;
    yahoo_hist_handler hist_handler;
    history_db history_db_;
    db::kv_db kv_db_;
};

template <typename yahoo_crawler, typename history_db>
void history_client<yahoo_crawler, history_db>::get_history_inc()
{
    cc_vec_string fname_vec;
    std::vector<common::timewrapper> endtime_vec;
    // 需要获取的时间区间
    self_type::get_endtime(endtime_vec);
    // csv文件名名称000001_2015-07-24.csv
    for (size_t idx = 0; idx < code_vec.size(); ++idx) {
        fname_vec.push_back(yahoo_data_path + code_vec[idx]
                + "_curr"
                + ".csv");
    }
    // 增量获取部分历史数据
    crawler.get_history_inc(code_vec, endtime_vec, fname_vec);
    // 处理历史数据
    self_type::handle_history(fname_vec, endtime_vec);
    // 更新每只股票已获得的历史数据的时间
    self_type::update_endtime(endtime_vec);
}

template <typename yahoo_crawler, typename history_db>
void history_client<yahoo_crawler, history_db>::handle_history(const std::vector<std::string>& fname_vec, std::vector<common::timewrapper>& endtime_vec)
{
    std::string date;
    std::vector<yahoo_hist> hist_vec;
    for (size_t idx = 0; idx < fname_vec.size(); ++idx) {
        // 获取csv文件数据
        date = hist_handler.parse_csv(fname_vec[idx], hist_vec);
        if (date != "") {
            endtime_vec[idx].add_day(-1);
            // 全部历史数据
            if (endtime_vec[idx].date_to_string() == BEGIN_DATE) {
                file_handler::rename_file(fname_vec[idx], yahoo_data_path + code_vec[idx] + "_" + date + ".csv");
            } 
            // 部分历史数据
            else {
                file_handler::merge_file(yahoo_data_path + code_vec[idx] + "_" + endtime_vec[idx].date_to_string() + ".csv", 
                        fname_vec[idx],
                        yahoo_data_path + code_vec[idx] + "_" + date + ".csv",
                        true);
            }
            endtime_vec[idx] = date;
        }
        else {
            // 返回初始的日期(数据库获得的)
            endtime_vec[idx].add_day(-1);
            // 删除无效的csv文件
            hist_handler.delete_csv_file(fname_vec[idx]);
        }
    }
}

template <typename yahoo_crawler, typename history_db>
void history_client<yahoo_crawler, history_db>::init_endtime()
{
    // 生成默认初始时间1980-01-01
    std::vector<common::timewrapper> endtime_vec(code_vec.size(), common::timewrapper(BEGIN_DATE));
    self_type::update_endtime(endtime_vec);
    // 删除已生成的csv文件
    common::file_handler::remove_all_file(yahoo_data_path);
}

template <typename yahoo_crawler, typename history_db>
void history_client<yahoo_crawler, history_db>::get_endtime(std::vector<common::timewrapper>& endtime_vec)
{
    history_db_.run(code_vec);
    endtime_vec.reserve(code_vec.size());
    std::string date;
    for (auto code : code_vec) {
        date = history_db_.yahoo_get_end(code);
        endtime_vec.push_back(common::timewrapper(date));
    }
}

template <typename yahoo_crawler, typename history_db>
void history_client<yahoo_crawler, history_db>::update_endtime(std::vector<common::timewrapper>& endtime_vec)
{
    if (kv_db_.open()) {
        for (size_t idx = 0; idx < code_vec.size(); ++idx) {
            kv_db_.insert(code_vec[idx], endtime_vec[idx].date_to_string());
        }
        kv_db_.close();
    }
}

}
#endif
