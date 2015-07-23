#include <iostream>
#include <leveldb/db.h>
#include <cassert>

void open_db()
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    // leveldb::Status status = leveldb::DB::Open(options, "~/Documents/temp", &db);
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
    }
    assert (status.ok());
}

void t_leveldb() {
    open_db();
}
