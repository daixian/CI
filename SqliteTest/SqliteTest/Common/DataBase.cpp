#pragma once
#include "DataBase.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include "SQLiteCpp/SQLiteCpp.h"
#pragma comment(lib, "SQLiteCpp.lib")
#pragma comment(lib, "sqlite3.lib")

#include "DBHelper.h"

#define LogI printf
#define LogW printf
#define LogE printf

using namespace std;
namespace fs = boost::filesystem;

namespace dxlib {

//内部数据
class DataBase::Impl
{
  public:
    Impl()
    {
    }
    ~Impl()
    {
        if (db != nullptr) {
            delete db;
        }
    }

    /// <summary> The database. </summary>
    SQLite::Database* db = nullptr;

  private:
};

DataBase::DataBase()
{
    _impl = new Impl{};
}

DataBase::~DataBase()
{
    delete _impl;
}

int DataBase::creatNewDB(const std::string& path)
{
    if (fs::is_regular_file(path)) { //如果文件存在
        LogE("DataBase.creatNewDB():数据库文件已经存在了,先确认手动删除之后再操作!");
        return -1;
    }
    try {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (_impl->db != nullptr) {
            LogI("DataBase.creatNewDB():创建数据库成功!");
            dbPath = path;
        }
        SQLite::Database& db = *(_impl->db);
        db.rekey(this->key); //设置加密

        //db.exec("DROP TABLE IF EXISTS table_name");
        //db.exec("CREATE TABLE table_name (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");

        //去掉多线程安全锁提高速度,需要保证单线程访问
        db.exec("PRAGMA synchronous = OFF;");
        db.exec("PRAGMA locking_mode = EXCLUSIVE;");
        //db.exec("PRAGMA DataBase_size = 4000;");
        //db.exec("PRAGMA temp_store = FILE;");
    }
    catch (const std::exception& e) {
        LogE("DataBase.creatNewDB():异常 %s", e.what());
        return -2;
    }

    return 1;
}

bool DataBase::openDB(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //如果文件存在
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READONLY);
        _impl->db->key(this->key); //加密
        if (_impl->db != nullptr) {
            LogI("DataBase.openDB():打开数据库成功!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //去掉多线程安全锁提高速度,需要保证单线程访问
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("DataBase.openDB():数据库文件不存在path=%s !", path.c_str());
    }
    LogE("DataBase.openDB():打开数据库失败!");
    return false;
}

bool DataBase::openDB_RW(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //如果文件存在
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE);
        _impl->db->key(this->key); //加密
        if (_impl->db != nullptr) {
            LogI("DataBase.openDB_RW():打开数据库成功!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //去掉多线程安全锁提高速度,需要保证单线程访问
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("DataBase.openDB_RW():数据库文件不存在path=%s !", path.c_str());
    }
    LogE("DataBase.openDB_RW():打开数据库失败!");
    return false;
}

void DataBase::closeDB()
{
    if (_impl->db != nullptr) {
        delete _impl->db;
        _impl->db = nullptr;
        dbPath.clear();
        LogI("DataBase.closeDB():关闭了数据库!");
        return;
    }
    else {
        LogE("DataBase.closeDB():数据库已经关闭,还未打开!");
    }
}

bool DataBase::isDBOpened()
{
    if (_impl->db != nullptr) {
        return true;
    }
    return false;
}

int DataBase::exec(const std::string& sql)
{
    return _impl->db->exec(sql);
}

} // namespace dxlib
