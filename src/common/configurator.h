/**
*Author: Steve Zhong
*Creation Date: 2015年06月23日 星期二 22时30分03秒
*Last Modified: 2015年08月01日 星期六 11时19分14秒
*Purpose: 程序配置器Boost::program_option封装
**/

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <string>
#include <vector>
#include <cstdlib>
#include <exception>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "logger.h"

namespace common {

namespace po = boost::program_options;
namespace pt = boost::property_tree;

class configurator {
public:
    configurator()
    {
        desc.add_options()
            ("help,h", "显示帮助信息")
        ;
    }
    bool add_plain_option(const char* name, const char* opt_desc)
    {
        desc.add_options()
            (name, opt_desc)
        ;
        return true;
    }
    template <typename T>
    bool add_option(const char* name, const char* opt_desc, T)
    {
        desc.add_options()
            (name, po::value<T>(), opt_desc)
        ;
        return true;
    }
    bool is_option_set(const std::string& name)
    {
        return vm.count(name);
    }
    template <typename T>
    T get_value(const std::string& path, T)
    {
        try {
            return tree.get<T>(path);
        }
        catch(pt::ptree_error& err) {
            // 文件路径不存在
            common::logger::parse_xml_error(err.what());
            exit(EXIT_FAILURE);
        }
        return T();
    }
    template <typename T>
    bool get_multi_value(const std::string& path, std::vector<T>& multi_val)
    {
        try {
            for(auto vt : tree.get_child(path)) {
                multi_val.push_back(vt.second.data());
            }
        }
        catch(pt::ptree_error& err) {
            common::logger::parse_xml_error(err.what());
        }
        return true;
    }
    std::string get_string_option(const std::string& name)
    {
        return vm[name].as<std::string>();
    }
    int get_int_option(const std::string& name)
    {
        return vm[name].as<int>();
    }
    bool parse_command_line(char argc, char* argv[])
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (is_option_set("config")) {
            pt::read_xml(get_string_option("config"), tree, pt::xml_parser::no_comments);
        }
        return true;
    }

    /* 解析一行程序选项
     * @param one: 是否只解析一个选项, 默认解析全部选项
     */
    bool parse_string_options(const std::string& str_cmd, bool one = false)
    {
        std::vector<std::string> args = po::split_unix(str_cmd);
        if (one && args.size() > 2) {
            // 删除后面的选项
            args.erase(args.begin() + 2, args.end());
        }
        try {
            po::store(po::command_line_parser(args).options(desc).run(), vm);
            po::notify(vm);
        }
        catch(po::error& err) {
            // 显示错误信息
            common::logger::log_info("未知选项：" + std::string(err.what()));
            return false;
        }
        return true;
    }

    bool parse_xml_file(const string& xml_file_name)
    {
        pt::read_xml(xml_file_name, tree, pt::xml_parser::no_comments);
        return true;
    }
    void show_options()
    {
        logger::log_info(desc);
    }
public:
    // 删除所有选项
    void delete_option_value()
    {
        vm = po::variables_map();
    }
private:
    po::options_description desc;
    po::variables_map vm;
    pt::ptree tree;
};

}

#endif
