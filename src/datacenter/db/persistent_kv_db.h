/**
*Author: Steve Zhong
*Creation Date: 2015年07月20日 星期一 21时52分51秒
*Last Modified: 2015年07月23日 星期四 00时08分15秒
*Purpose: leveldb持久化存储
**/

#ifndef PERSISTENT_KV_DB_H_
#define PERSISTENT_KV_DB_H_

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <cctype>

#include <leveldb/db.h>

#include <common/logger.h>

namespace datacenter {
namespace db {

using db_handler    = leveldb::DB;
using db_status     = leveldb::Status;
using db_options    = leveldb::Options;

// 内存数据库，方便快速查找验证

class kv_db {
    using logger = common::logger;
public:
    ~kv_db()
    {
        close();
    }
    // 配置数据库位置
    void configure(const std::string& db_path_)
    {
        db_path = db_path_;
        // 不存在则创建
        options.create_if_missing = true;
    }
    // 打开数据库
    bool open()
    {
        db_status status = db_handler::Open(options, db_path, &handler); 
        if (status.ok())
            return true;
        return false;
    }
    // 插入数据
    template <typename Key, typename Value>
    bool insert(Key& key, const Value& value)
    {
        db_status status = handler->Put(leveldb::WriteOptions(), key, value);
        return status.ok();
    }
    // 读取数据
    template <typename Key, typename Value>
    bool select(Key& key, const Value& value)
    {
        db_status status = handler->Get(leveldb::WriteOptions(), key, &value);
        return status.ok();
    }
    // 删除数据
    template <typename Key>
    bool del(const Key& key)
    {
        db_status status = handler->Delete(leveldb::WriteOptions(), key);
        return status.ok();
    }
    // 关闭数据库
    void close()
    {
        delete handler;
    }
private:
    // 数据库位置
    std::string db_path;
    // 数据库选项
    db_options options;
    // 数据库句柄
    db_handler *handler;
};

}
}

#endif
