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

//�ڲ�����
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
    if (fs::is_regular_file(path)) { //����ļ�����
        LogE("Cache.creatNewDB():���ݿ��ļ��Ѿ�������,��ȷ���ֶ�ɾ��֮���ٲ���!");
        return -1;
    }
    try {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        if (_impl->db != nullptr) {
            LogI("Cache.creatNewDB():�������ݿ�ɹ�!");
            dbPath = path;
        }
        SQLite::Database& db = *(_impl->db);
        db.rekey(this->key); //���ü���

        //ɾ�����ڵı�
        db.exec("DROP TABLE IF EXISTS sht_data");
        //�����ʹ�õı�(һ��������,һ������������)
        db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");

        //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
        db.exec("PRAGMA synchronous = OFF;");
        db.exec("PRAGMA locking_mode = EXCLUSIVE;");
        //db.exec("PRAGMA cache_size = 4000;");
        //db.exec("PRAGMA temp_store = FILE;");
    }
    catch (const std::exception& e) {
        LogE("Cache.creatNewDB():�쳣 %s", e.what());
        return -2;
    }

    return 1;
}

bool Cache::openDB(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //����ļ�����
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READONLY);
        _impl->db->key(this->key); //����
        if (_impl->db != nullptr) {
            LogI("Cache.openDB():�����ݿ�ɹ�!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("Cache.openDB():���ݿ��ļ�������path=%s !", path.c_str());
    }
    LogE("Cache.openDB():�����ݿ�ʧ��!");
    return false;
}

bool Cache::openDB_RW(const std::string& path)
{
    if (_impl->db != nullptr) {
        delete _impl->db;
    }
    //����ļ�����
    if (fs::is_regular_file(path)) {
        _impl->db = new SQLite::Database(path, SQLite::OPEN_READWRITE);
        _impl->db->key(this->key); //����
        if (_impl->db != nullptr) {
            LogI("Cache.openDB_RW():�����ݿ�ɹ�!");
            dbPath = path;

            SQLite::Database& db = *(_impl->db);
            //ȥ�����̰߳�ȫ������ٶ�,��Ҫ��֤���̷߳���
            db.exec("PRAGMA synchronous = OFF;");
            db.exec("PRAGMA locking_mode = EXCLUSIVE;");
            return true;
        }
    }
    else {
        LogE("Cache.openDB_RW():���ݿ��ļ�������path=%s !", path.c_str());
    }
    LogE("Cache.openDB_RW():�����ݿ�ʧ��!");
    return false;
}

void Cache::closeDB()
{
    if (_impl->db != nullptr) {
        delete _impl->db;
        _impl->db = nullptr;
        dbPath.clear();
        LogI("Cache.closeDB():�ر������ݿ�!");
        return;
    }
    else {
        LogE("Cache.closeDB():���ݿ��Ѿ��ر�,��δ��!");
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
    query.bind(1, name);                     //��������
    query.bindNoCopy(2, data.data(), data.size()); //byte����,ע������ʹ���˲�Copy�ķ���,��Ҫ��֤data����������
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
    //ɾ�����ڵı�
    db.exec("DROP TABLE IF EXISTS sht_data");
    //���´��������ʹ�õı�
    return db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, data_name TEXT, data_byte BLOB)");
}

} // namespace sht
