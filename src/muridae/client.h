/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月22日 星期一 11时54分32秒
*Purpose:
**/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <string>

#include "crawler.h"
#include "stock.h"
#include "../tiger/cmd_display.h"

namespace muridae {
	
class client {
public:
	using displayer = tiger::cmd_display;

public:
	void run(int argc, char* argv[]) {
		crawler_.config(argc, argv);
		crawler_.run(stock_vec);
		display();
	}
	
private:
	void display() {
		displayer::stock_info(stock_vec);
	}

private:
	crawler crawler_;
	vector<stock> stock_vec;
};

}
#endif
