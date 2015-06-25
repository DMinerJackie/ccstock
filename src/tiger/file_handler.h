/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 21时15分57秒
*Last Modified: 2015年06月22日 星期一 23时58分48秒
*Purpose:
**/

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <fstream>

#include "io_aux.h"
#include "../muridae/stock.h"

namespace tiger {
class file_handler {
public:
    using stock     = muridae::stock;
    using io_aux    = tiger::io_aux;
public:
    static bool save_code_name(std::vector<stock>& stock_vec, const string& fname)
    {
        string dir_path("data/code/");
        io_aux::create_folder(dir_path.c_str());
        ofstream ifs(dir_path + fname, ios::out);
        
        for (auto stock : stock_vec) {
            io_aux::out_variadic(ifs, stock.code, ", ", stock.name);
        }

        return true;
    }
};

}
#endif
