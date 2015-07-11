/**
*Author: Steve Zhong
*Creation Date: 2015年07月11日 星期六 17时16分48秒
*Last Modified: 2015年07月12日 星期日 00时21分27秒
*Purpose:
**/
#ifndef TEXT_BASED_INTERFACE_H
#define TEXT_BASED_INTERFACE_H

#include <vector>
#include <cstring>
#include <clocale>

#include <cstdio>

#include <common/common_defs.h>
#include <simulator/instrument/stock.h>

#include "systime.h"
#include "ui_common.h"

#include "ncurses.h"

namespace simulator {
namespace ui {

class text_based_interface {
public:
    text_based_interface()
    {
        setlocale(LC_ALL, "zh_CN.UTF-8"); // 支持中文

        initscr(); // start curses mode 
        win_handle = stdscr;
        getmaxyx(win_handle, row_, col_);        
    }

    void stock_info_basic(const std::vector<stock>& stock_vec)
    {
        std::vector<const char*> strvec = {common::CODE_CH,
            common::NAME_CH,
            common::CURR_P,
            common::INC,
            common::INC_V,
            common::OPEN_P,
            common::CLOSE_P,
            common::HIGH_P,
            common::LOW_P};        
        int row = 0, col = 0;
        mvprintw(row++, col, "%s    %s", "自选股行情", systime::get_curr_time());

        attron(A_BOLD);
        for (auto str : strvec) {
            mvaddstr(row, col, str);
            col += 10; 
        }
        attroff(A_BOLD);

        row++;
        for (auto one : stock_vec) {
            col = 0;
            mvprintw(row, col, "%s", one.code.c_str()); // 股票代码
            col += 10;
            mvprintw(row, col, "%s", one.name.c_str()); // 股票名称
            col += 10;
            print_double(row, col, one.curr_price, one.curr_price - one.close_price);
            col += 10;
            print_string(row, col, one.inc, &ui_common::get_inc);
            col += 10;
            print_double(row, col, one.inc_v, one.inc_v);
            col += 10;
            print_double(row, col, one.open_price, one.open_price - one.close_price);
            col += 10;
            mvprintw(row, col, "%.2lf", one.close_price);
            col += 10;
            print_double(row, col, one.high_price, one.high_price - one.close_price);
            col += 10;
            print_double(row, col, one.low_price, one.low_price - one.close_price);
            col += 10;
            ++row;
        }
        refresh();
    }
    ~text_based_interface()
    {
        endwin();
    }
private:
    void print_string(int row, int col, const double& val, process_double_t proc_double_func) const 
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        if (val > 0) {
            attron(COLOR_PAIR(1));
            mvaddstr(row, col, proc_double_func(val).c_str());
            attroff(COLOR_PAIR(1)); 
        }
        else if (val < 0) 
        {
            attron(COLOR_PAIR(2));
            mvaddstr(row, col, proc_double_func(val).c_str());
            attroff(COLOR_PAIR(2)); 
        } else
        {
            attron(COLOR_PAIR(3));
            mvaddstr(row, col, proc_double_func(val).c_str());
            attroff(COLOR_PAIR(3)); 
        }
    }
    void print_double(int row, int col, const double& val, double const& flag) const 
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        if ((val > -0.01 && val < 0.01) || (flag > -0.01 && flag < 0.01)) {
            attron(COLOR_PAIR(3));
            mvprintw(row, col, "%.2lf", val);
            attroff(COLOR_PAIR(3)); 
        } 
        else if (flag >= 0.01) {
                attron(COLOR_PAIR(1));
                mvprintw(row, col, "%.2lf", val);
                attroff(COLOR_PAIR(1)); 
        }
        else if (flag <= -0.01) {
                attron(COLOR_PAIR(2));
                mvprintw(row, col, "%.2lf", val);
                attroff(COLOR_PAIR(2)); 
        } 
    }
private:
    int row_, col_;
    WINDOW* win_handle;
};

}
}

#endif