/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年06月25日 星期四 22时08分08秒
*Purpose:
**/

#include <iostream>
#include <string>

#include "ccstock_config.h"

class stock_service {
public:
    stock_service(configurator* config_): client_(config_)
    {
    }
    void show_market_data(const std::string& code)
    {
        client_.show_md(code);
    }
    void gen_code_name(const std::string& bk)
    {
        client_.gen_code(bk);
    }
private:
    client client_;
};

int main(int argc, char* argv[]) {
    configurator *configurator_ = new configurator();
    configurator_->add_option("code,a", "stock's code, separate by ','", std::string());
    configurator_->add_option("gen,g", "generate market's code and name, eg sh, sz,.etc", std::string());
    configurator_->add_option("config,c", "config file", std::string());
    configurator_->parse_command_line(argc, argv);

    if (configurator_->is_option_set("help")) {
        configurator_->show_options();
        return 0;
    }

	stock_service service(configurator_);
    if (configurator_->is_option_set("code")) {
        service.show_market_data(configurator_->get_string_option("code"));        
    }
    if (configurator_->is_option_set("gen")) {
        service.gen_code_name(configurator_->get_string_option("gen"));
    }

	return 0;
}
