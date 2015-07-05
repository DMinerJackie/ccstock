/**
*Author: Steve Zhong
*Creation Date: 2015年06月23日 星期二 22时30分03秒
*Last Modified: 2015年07月02日 星期四 00时09分04秒
*Purpose:
**/

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <string>

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
    }
    bool is_option_set(const std::string& name)
    {
        return vm.count(name);
    }
    template <typename T>
    T get_value(const std::string& path, T)
    {
        return tree.get<T>(path);
    }
    template <typename T>
    bool get_multi_value(const std::string& path, std::vector<T>& multi_val)
    {
        for(auto vt : tree.get_child(path)) {
            multi_val.push_back(vt.second.data());
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
    void show_options()
    {
        logger::log_info(desc);
    }
private:
    po::options_description desc;
    po::variables_map vm;
    pt::ptree tree;
};

}

#endif
