#include "pch.h"
#include "Cache.h"
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include "SQLiteCpp/SQLiteCpp.h"
#pragma comment(lib, "SQLiteCpp.lib")
#pragma comment(lib, "sqlite3.lib")

#include "./Common/DBHelper.h"

#define LogI printf
#define LogE printf

using namespace std;
namespace fs = boost::filesystem;

namespace sht {

Cache* Cache::m_pInstance = nullptr;

//内部数据
class Cache::Impl
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

Cache::Cache()
{
    _impl = new Impl{};
}

Cache::~Cache()
{
    delete _impl;
}

int Cache::creatNewDB(const std::string& path)
{
    if (fs::is_regular_file(path)) { //如果文件存在
        LogE("Cache.creatNewDB():数据库文件已经存在了,先确认手动删除之后再操作!");
        return -1;
    }
    try {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (_impl->db != nullptr) {
            LogI("Cache.creatNewDB():创建数据库成功!");
            dbPath = path;
        }
        SQLite::Database& db = *(_impl->db);
        db.rekey(this->key); //设置加密

        //删除存在的表
        db.exec("DROP TABLE IF EXISTS sht_data");
        //陕昊天使用的表(一个数据名,一个二进制数据)
        db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");

        //去掉多线程安全锁提高速度,需要保证单线程访问
        db.exec("PRAGMA synchronous = OFF;");
        db.exec("PRAGMA locking_mode = EXCLUSIVE;");
        //db.exec("PRAGMA cache_size = 4000;");
        //db.exec("PRAGMA temp_store = FILE;");
    }
    catch (const std::exception& e) {
        LogE("Cache.creatNewDB():异常 %s", e.what());
        return -2;
    }

    return 1;
}

bool Cache::openDB(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //如果文件存在
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READONLY);
        _impl->db->key(this->key); //加密
        if (_impl->db != nullptr) {
            LogI("Cache.openDB():打开数据库成功!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //去掉多线程安全锁提高速度,需要保证单线程访问
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("Cache.openDB():数据库文件不存在path=%s !", path.c_str());
    }
    LogE("Cache.openDB():打开数据库失败!");
    return false;
}

bool Cache::openDB_RW(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //如果文件存在
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE);
        _impl->db->key(this->key); //加密
        if (_impl->db != nullptr) {
            LogI("Cache.openDB_RW():打开数据库成功!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //去掉多线程安全锁提高速度,需要保证单线程访问
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("Cache.openDB_RW():数据库文件不存在path=%s !", path.c_str());
    }
    LogE("Cache.openDB_RW():打开数据库失败!");
    return false;
}

void Cache::closeDB()
{
    if (_impl->db != nullptr) {
        delete _impl->db;
        _impl->db = nullptr;
        dbPath.clear();
        LogI("Cache.closeDB():关闭了数据库!");
        return;
    }
    else {
        LogE("Cache.closeDB():数据库已经关闭,还未打开!");
    }
}

bool Cache::isDBOpened()
{
    if (_impl->db != nullptr) {
        return true;
    }
    return false;
}

int Cache::exec(const std::string& sql)
{
    return _impl->db->exec(sql);
}

int Cache::addCacheData(const std::string& name, const std::vector<char> data)
{
    SQLite::Database& db = *(_impl->db);
    SQLite::Statement query(db, "INSERT INTO sht_data VALUES (NULL,?,?)");
    query.bind(1, name);                     //数据名字
    query.bindNoCopy(2, data.data(), data.size()); //byte数据,注意这里使用了不Copy的方法,需要保证data的生命周期
    int nb = query.exec();
    return nb;
}

int Cache::getCacheData(const std::string& name, std::vector<char>& data)
{
    using namespace dxlib;

    SQLite::Database& db = *(_impl->db);

    std::string table_name = "sht_data";
    std::string where_col_name = "data_name";
    std::string where_row_value = name;

    std::string json;
    int nb = DBHelper::SELECT(_impl->db, table_name, where_col_name, where_row_value, "data_byte", data);
    return nb;
}

int Cache::clearCacheData()
{
    SQLite::Database& db = *(_impl->db);
    //删除存在的表
    db.exec("DROP TABLE IF EXISTS sht_data");
    //重新创建陕昊天使用的表
    return db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");
}

} // namespace sht
