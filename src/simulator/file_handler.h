/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 21时15分57秒
*Last Modified: 2015年06月27日 星期六 23时52分17秒
*Purpose:
**/

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "io_aux.h"
#include "../gateway/stock.h"
#include "../common/utility.h"

namespace simulator {
class file_handler {
public:
    using self_type = file_handler;
    using stock     = gateway::stock;
    using io_aux    = simulator::io_aux;
    using utility   = common::utility;
public:
    static bool save_code_name(std::vector<stock>& stock_vec, const string& dir_path, const string& fname)
    {
        io_aux::create_folder(dir_path.c_str());
        ofstream ofs(dir_path + fname, ios::out);
        
        for (auto stock : stock_vec) {
            io_aux::out_variadic(ofs, stock.code, " ", stock.name);
        }
        ofs.close();
        return true;
    }
    static bool read_code(std::vector<std::string>& code_vec, const string& path, const string& bk)
    {
        ifstream ifs(path + bk + "_code_name.ds");
        string code_name;
        while (getline(ifs, code_name)) {
            code_vec.push_back(code_name.substr(0, 6));
        }
        ifs.close();
        return true;
    }
    static bool add_option(const std::vector<std::string>& code_vec, const string& path)
    {
        std::vector<std::string> option_vec;
        self_type::read_option(option_vec, path); 
        ofstream ofs(path + "option.ds", std::ios::app);
        for (auto code : code_vec) {
            if (std::binary_search(option_vec.begin(), option_vec.end(), code) == false) {
                ofs << code << endl;
            }
        }
        ofs.close();
        return true;
    }
    static bool del_option(std::vector<std::string>& code_vec, const string& path)
    {
        std::vector<std::string> option_vec;
        self_type::read_option(option_vec, path); 
        utility::remove_common_vector(option_vec, code_vec);
        ofstream ofs(path + "option.ds");
        for (auto code : option_vec) {
            ofs << code << endl;
        }
        ofs.close();
        return true;
    }
    static bool read_option(std::vector<std::string>& code_vec, const string& path)
    {
        ifstream ifs(path + "option.ds");
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
