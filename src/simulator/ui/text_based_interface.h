/**
*Author: Steve Zhong
*Creation Date: 2015年07月11日 星期六 17时16分48秒
*Last Modified: 2015年07月11日 星期六 17时24分19秒
*Purpose:
**/
#ifndef TEXT_BASED_INTERFACE_H
#define TEXT_BASED_INTERFACE_H

#include "ncurses.h"

namespace simulator {
namespace ui {

class text_based_interface {
public:
    text_based_interface()
    {
       initscr(); // start curses mode 
    }

    void display()
    {
        refresh();
    }
    ~text_based_interface()
    {
        endwin();
    }
private:

};

}
}
#endif
