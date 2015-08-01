/**
*Author: Steve Zhong
*Creation Date: 2015年06月22日 星期一 00时13分41秒
*Last Modified: 2015年08月01日 星期六 10时50分50秒
*Purpose: 行情模拟器
**/

#ifndef MDSIM_H_
#define MDSIM_H_

#include <gateway/market_data_crawler.h>
#include <gateway/code_crawler.h>

#include <simulator/market_data_client.h>
#include <simulator/code_initializer.h>
#include <simulator/option_manager.h>
#include <simulator/instrument/code_db.h>

#include <simulator/ui/text_based_interface.h>

#include <common/configurator.h>

class mdsim_service {
    using code_db                   = simulator::code_db;
    using md_crawler                = gateway::market_data_crawler;
    using tb_displayer              = simulator::ui::text_based_interface;
    
    using md_client_t               = simulator::market_data_client<code_db, md_crawler, tb_displayer>;
    using option_manager_t          = simulator::option_manager<code_db, md_crawler, tb_displayer>;
    
    using code_crawler              = gateway::code_crawler;
    using code_initializer_t        = simulator::code_initializer<code_crawler>;
    
    using configurator              = common::configurator;
public:
    mdsim_service(shared_ptr<configurator> config_):
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
    // 显示板块股票行情
    void show_market_data(const std::string& bk, uint32_t speed, uint32_t top_num, std::string& order)
    {
        md_client_.show_md_bk(bk, speed, top_num, order);
    }
    // 增加自选股
    void add_option(const std::string& options)
    {
        option_manager_.add_option(options);
    }
    // 删除自选股
    void del_option(const std::string& options)
    {
        option_manager_.del_option(options);
    }
    // 显示自选股行情
    void show_option()
    {
        option_manager_.show_option();
    }
    // 手动初始化股票代码
    void code_initialize()
    {
        code_initializer_.manual_initialize();
    }
private:
    // 行情客户端
    md_client_t md_client_;
    // 股票代码初始化器
    code_initializer_t code_initializer_;
    // 自选股管理器
    option_manager_t option_manager_;
    // 股票代码数据
    shared_ptr<code_db> db;
private:
    std::string code_path;
};

#endif
