/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 21时15分57秒
*Last Modified: 2015年07月09日 星期四 10时43分32秒
*Purpose:
**/

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <algorithm>

#include "common/io_aux.h"
#include "common/utility.h"
#include "common/common_defs.h"

#include "instrument/stock.h"

namespace simulator {
class file_handler {
public:
    using self_type = file_handler;
    using io_aux    = common::io_aux;
    using utility   = common::utility;
public:
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
        ofstream ofs(path + "option.ds", std::ios::app);
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
        ofstream ofs(path + "option.ds");
        for (auto code : option_vec) {
            ofs << code << endl;
        }
        ofs.close();
        return true;
    }
    static bool read_option(cc_vec_string& code_vec, const string& path)
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
