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

//�ڲ�����
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
    if (fs::is_regular_file(path)) { //����ļ�����
        LogE("DataBase.creatNewDB():���ݿ��ļ��Ѿ�������,��ȷ���ֶ�ɾ��֮���ٲ���!");
        return -1;
    }
    try {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (_impl->db != nullptr) {
            LogI("DataBase.creatNewDB():�������ݿ�ɹ�!");
            dbPath = path;
        }
        SQLite::Database& db = *(_impl->db);
        db.rekey(this->key); //���ü���

        //db.exec("DROP TABLE IF EXISTS table_name");
        //db.exec("CREATE TABLE table_name (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");

        //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
        db.exec("PRAGMA synchronous = OFF;");
        db.exec("PRAGMA locking_mode = EXCLUSIVE;");
        //db.exec("PRAGMA DataBase_size = 4000;");
        //db.exec("PRAGMA temp_store = FILE;");
    }
    catch (const std::exception& e) {
        LogE("DataBase.creatNewDB():�쳣 %s", e.what());
        return -2;
    }

    return 1;
}

bool DataBase::openDB(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //����ļ�����
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READONLY);
        _impl->db->key(this->key); //����
        if (_impl->db != nullptr) {
            LogI("DataBase.openDB():�����ݿ�ɹ�!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("DataBase.openDB():���ݿ��ļ�������path=%s !", path.c_str());
    }
    LogE("DataBase.openDB():�����ݿ�ʧ��!");
    return false;
}

bool DataBase::openDB_RW(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //����ļ�����
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE);
        _impl->db->key(this->key); //����
        if (_impl->db != nullptr) {
            LogI("DataBase.openDB_RW():�����ݿ�ɹ�!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("DataBase.openDB_RW():���ݿ��ļ�������path=%s !", path.c_str());
    }
    LogE("DataBase.openDB_RW():�����ݿ�ʧ��!");
    return false;
}

void DataBase::closeDB()
{
    if (_impl->db != nullptr) {
        delete _impl->db;
        _impl->db = nullptr;
        dbPath.clear();
        LogI("DataBase.closeDB():�ر������ݿ�!");
        return;
    }
    else {
        LogE("DataBase.closeDB():���ݿ��Ѿ��ر�,��δ��!");
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
