/**
*Author: Steve Zhong
*Creation Date: 2015年08月03日 星期一 22时12分41秒
*Last Modified: 2015年08月03日 星期一 22时55分48秒
*Purpose: 利用CURL发送电子邮件
**/

#ifndef EMAIL_H_
#define EMAIL_H_

#include <string>
#include <cstdlib>
#include <vector>

#include <common/configurator.h>

namespace as {
namespace tools {

class email {
    using configurator = common::configurator;
public:
    email()
    {
        config = new configurator();
    }
    ~email()
    {
        delete config;
    }
    // 配置发送者接受者信息
    bool configure(std::string xml_file_name)
    {
        // 读入xml配置文件
        config->parse_xml_file(xml_file_name);
        username = config->get_value("mail.sender.username", std::string());
        smtp_server = config->get_value("mail.sender.smtpserver", std::string());
        port = config->get_value("mail.sender.port", std::string());
        pwd = config->get_value("mail.sender.password", std::string());
        // 接收者邮箱
        config->get_multi_value("mail.recv.address", recv_set);
        // 正文文件名
        body_name = config->get_value("mail.body", std::string());
        return true;
    }
    void send()
    {
        for (auto recv : recv_set)
        {
            gen_cmd(recv);
            gen_body(recv);
            system(cmd.c_str());
        }
    }
private:
    // 生成curl发送邮件命令
    void gen_cmd(std::string& recv_addr)
    {       
        cmd += "curl ";
        cmd += "--url \"smtps://" + smtp_server + ":" + port + "\" ";
        cmd += "--ssl-reqd ";
        cmd += "--mail-from \"" + username + "\" "
        cmd += "--mail-rcpt \"" + recv_addr + "\" ";
        cmd += "--upload-file " + body_name;
        cmd += "--user \"" + username + ":" + pwd + "\" "; 
        cmd += "--insecure";
    }
    // 生成邮件正文
    void gen_body(const std::string& recv_addr)
    {
        // Todo
    }
private:
    configurator *config;
private:
    // 发送者相关配置
    std::string username;
    std::string smtp_server;
    std::string port;
    std::string pwd;
    // 接受者邮箱地址
    std::vector<std::string> recv_set; // 接受者邮件地址
    // 发送邮件命令
    std::string cmd;
    // 正文内容文件
    std::string body_name;
};

}
}

#endif
