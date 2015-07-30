/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 21时15分57秒
*Last Modified: 2015年07月30日 星期四 22时33分30秒
*Purpose:
**/

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include <boost/filesystem.hpp>

#include <common/logger.h>
#include <common/io_aux.h>
#include <common/utility.h>
#include <common/common_defs.h>

#include <simulator/instrument/stock.h>

namespace common {

namespace fs          = boost::filesystem;

class file_handler {
public:
    using self_type   = file_handler;
    using stock       = simulator::stock;
    using stock_basic = simulator::stock_basic;
    
public:
    // 删除文件
    static bool remove_file(const std::string& filename)
    {
        return remove(filename.c_str()) == 0;
    }
    // 删除目录中所有文件
    static int remove_all_file(const std::string& path)
    {
        // 调用bash命令
        std::string command("exec rm -r " + path + "*");
        return system(command.c_str());
    }
    // 重命名文件
    static bool rename_file(const string& oldname, const string& newname)
    {
        return rename(oldname.c_str(), newname.c_str()) == 0;
    }
    // 合并文件(file1->file2)
    static void merge_file(const string& filename1, const string& filename2, const string& new_filename = "", bool delete_file1 = false)
    {
        std::ifstream ifs(filename1);
        std::ofstream ofs(filename2);
        ofs << ifs.rdbuf();
        ofs.close();
        // 删除文件1
        if (delete_file1) {
            self_type::remove_file(filename1);
        }
        // 重命名文件
        if (new_filename != "") {
            self_type::rename_file(filename1, new_filename);
        }
    }
    // 获取得到文件列表
    static void get_file_list(std::vector<std::string>& file_list, const std::string& path)
    {
        if (fs::exists(path.c_str())) {
            fs::directory_iterator end_iter;
            for (fs::directory_iterator dir_iter(path); dir_iter != end_iter; ++dir_iter) {
                if (fs::is_regular_file(*dir_iter) && dir_iter->path().extension() == ".csv") {
                    file_list.push_back(dir_iter->path().filename().string());
                }
            }
        }
        else {
            common::logger::dir_not_exist(path);
        }
    }
    static bool save_code(cc_vec_string& code_vec, const std::string& dir_path, const std::string& fname)
    {
        io_aux::create_folder(dir_path.c_str());
        ofstream ofs(dir_path + fname, ios::out);
        for (auto code : code_vec) {
            io_aux::out_variadic(ofs, code);
        }
        ofs.close();
        return true;
    }
    static bool save_code_jp_name(std::vector<stock_basic>& stock_basic_vec, const string& dir_path)
    {
        io_aux::create_folder(dir_path.c_str());
        ofstream ofs(dir_path + "code_jp_name.ds", ios::out);
        std::string data;
        for (auto stock_basic : stock_basic_vec) {
            io_aux::out_variadic(ofs,
                    stock_basic.code, " ",
                    stock_basic.jp, " ",
                    stock_basic.name, " ",
                    stock_basic.market_code);
        }
        ofs.close();
        return true;
    }
    // 读股票代码数据
    static bool read_code_jp_name(std::vector<stock_basic>& stock_basic_vec, const string& dir_path)
    {
        FILE *ifp = fopen((dir_path + "code_jp_name.ds").c_str(), "r");
        char code[7], jp[7], name[20], market_code[10];
        while (fscanf(ifp, "%s %s %s %s", code, jp, name, market_code) != EOF) {
            stock_basic_vec.push_back(stock_basic(code, jp, name, market_code));
        }
        fclose(ifp);
        return true;
    }
    static bool read_code(cc_vec_string& code_vec, const string& path, const string& bk)
    {
        ifstream ifs(path + bk + "_code.ds");
        string code_name;
        while (getline(ifs, code_name)) {
            code_vec.push_back(code_name.substr(0, 6));
        }
        ifs.close();
        return true;
    }
    // 增加自选股
    static bool add_option(const cc_vec_string& code_vec, const string& path)
    {
        cc_vec_string option_vec;
        self_type::read_option(option_vec, path);
        ofstream ofs(path + "option_code.ds", std::ios::app);
        for (auto code : code_vec) {
            if (std::binary_search(option_vec.begin(), option_vec.end(), code) == false) {
                ofs << code << endl;
            }
        }
        ofs.close();
        return true;
    }
    static bool del_option(cc_vec_string& code_vec, const string& path)
    {
        cc_vec_string option_vec;
        self_type::read_option(option_vec, path);
        utility::remove_common_vector(option_vec, code_vec);
        ofstream ofs(path + "option_code.ds");
        for (auto code : option_vec) {
            ofs << code << endl;
        }
        ofs.close();
        return true;
    }
    static bool read_option(cc_vec_string& code_vec, const string& path)
    {
        ifstream ifs(path + "option_code.ds");
        string code;
        while (ifs >> code) {
            code_vec.push_back(code);
        }
        utility::unique_vector(code_vec);
        ifs.close();
        return true;
    }
};

}
#endif
