/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年07月19日 星期日 19时49分32秒
*Purpose:
**/

#include <iostream>
#include <string>
#include <memory> // shared_ptr

#include "ccstock_config.h"

class stock_service {
public:
    stock_service(shared_ptr<configurator> config_): 
        md_client_(config_),
        code_initializer_(config_),
        db(new code_db)
    {
        // 生成股票代码相关文件
        code_initializer_.run();
        // 股票代码存放路径
        code_path = config_->get_value("stock.market_data.code.code_path", std::string());
        // 读取所有上市公司股票代码信息
        db->configure(code_path);
        // 初始化行情客户端
        md_client_.initialize();
        //自选股管理器
        option_manager_.configure(db, 
                config_->get_value("stock.market_data.code.option_path", std::string()));
        // 历史数据客户端
        history_client_.configure(code_path, 
                config_->get_value("stock.market_data.code.history_path", std::string()));
    }
    // 显示个股行情
    void show_stock_data(const std::string& code)
    {
        md_client_.show_stock(code);
    }
    // 显示大盘行情
    void show_market()
    {
        md_client_.show_market();
    }
    void show_market_data(const std::string& bk, uint32_t speed, uint32_t top_num, std::string& order)
    {
        md_client_.show_md_bk(bk, speed, top_num, order);
    }
    void add_option(const std::string& options)
    {
        option_manager_.add_option(options);
    }
    void del_option(const std::string& options)
    {
        option_manager_.del_option(options);
    }
    void show_option()
    {
        option_manager_.show_option();
    }
    // 获取历史数据
    void get_history_data()
    {
        history_client_.get_history_all();
    }
    // 手动初始化股票代码
    void code_initialize()
    {
        code_initializer_.manual_initialize();
    }
private:
    md_client_t md_client_;
    code_initializer_t code_initializer_;
    option_manager_t option_manager_;
    history_client_t history_client_;
    // 股票代码数据
    shared_ptr<code_db> db;
private:
    std::string code_path;
};

int main(int argc, char* argv[]) {
    shared_ptr<configurator> configurator_ (new configurator());
    configurator_->add_option("code,a", "查看个股信息，代码用','分开", std::string());
    configurator_->add_option("config,c", "设置配置文件", std::string());
    configurator_->add_option("data,d", "批量显示行情", std::string());
    configurator_->add_plain_option("market,m", "显示大盘行情");
    configurator_->add_option("top,t", "筛选前几个信息", int());
    configurator_->add_option("order,o", "排序规则", std::string());
    configurator_->add_option("add-option", "追加自选股,股票代码用','分开", std::string());
    configurator_->add_option("del-option", "删除自选股,股票代码用','分开", std::string());
    configurator_->add_plain_option("show-option,O", "查看自选股");
    configurator_->add_plain_option("history,H", "获取历史数据");
    configurator_->add_plain_option("init", "初始化股票代码");
    configurator_->parse_command_line(argc, argv);

    if (configurator_->is_option_set("help")) {
        configurator_->show_options();
        return 0;
    }

    stock_service service(configurator_);
    if (configurator_->is_option_set("code")) {
        service.show_stock_data(configurator_->get_string_option("code"));        
    }
    if (configurator_->is_option_set("market")) {
        service.show_market();
    }
    if (configurator_->is_option_set("add-option")) {
        service.add_option(configurator_->get_string_option("add-option"));
    }
    if (configurator_->is_option_set("del-option")) {
        service.del_option(configurator_->get_string_option("del-option"));
    }
    if (configurator_->is_option_set("show-option")) {
        service.show_option();
    }
    if (configurator_->is_option_set("history")) {
        service.get_history_data();
    }
    if (configurator_->is_option_set("init")) {
        service.code_initialize();
    }
    if (configurator_->is_option_set("data")) {
        int32_t top_num = -1;
        std::string order("");
        if (configurator_->is_option_set("top")) {
            top_num = configurator_->get_int_option("top");
        }
        if (configurator_->is_option_set("order")) {
            order= configurator_->get_string_option("order");
        }
        service.show_market_data(configurator_->get_string_option("data"),
                configurator_->get_value("stock.market_data.request_speed", uint32_t()),
                top_num,
                order);
    }

    return 0;
}
