/**
*Author: Steve Zhong
*Creation Date: 2015年06月23日 星期二 22时30分03秒
*Last Modified: 2015年06月25日 星期四 21时43分57秒
*Purpose:
**/

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <string>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "logger.h"

namespace cow {

namespace po = boost::program_options;
namespace pt = boost::property_tree;

class configurator {
public:
    configurator() 
    {
        desc.add_options()
            ("help,h", "program help message")
        ;
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
    bool parse_command_line(char argc, char* argv[])
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (is_option_set("config")) {
            pt::read_xml(get_string_option("config"), tree);
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
