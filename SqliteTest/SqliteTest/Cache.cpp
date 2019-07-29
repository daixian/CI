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
        db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, frame_num INTEGER, set_name TEXT, set_type TEXT, data_byte BLOB)");

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

int Cache::addCacheData(const Item& item)
{
    SQLite::Database& db = *(_impl->db);
    SQLite::Statement query(db, "INSERT INTO sht_data VALUES (NULL,?,?,?,?)");
    query.bind(1, item.frameNum);
    query.bind(2, item.setName);
    query.bind(3, item.setType);
    query.bindNoCopy(4, item.data.data(), item.data.size()); //byte����,ע������ʹ���˲�Copy�ķ���,��Ҫ��֤data����������
    int nb = query.exec();
    return nb;
}

int Cache::getCacheData(Item& item)
{
    using namespace dxlib;

    SQLite::Database& db = *(_impl->db);
    string sql = (boost::format("SELECT data_byte FROM sht_data WHERE frame_num='%d'") % item.frameNum).str();

    if (!item.setName.empty() && !item.setType.empty())
        sql = (boost::format("SELECT data_byte FROM sht_data WHERE frame_num='%d' AND set_name='%s' AND set_type='%s'") % item.frameNum % item.setName % item.setType)
                  .str();
    else if (!item.setName.empty() && item.setType.empty()) //���ֻ����setName
        sql = (boost::format("SELECT data_byte FROM sht_data WHERE frame_num='%d' AND set_name='%s'") % item.frameNum % item.setName)
                  .str();

    //�����ݿ����Ϣ
    SQLite::Statement querybr(db, sql);
    while (querybr.executeStep()) {
        if (querybr.getColumn(0).isBlob()) {
            int bytes = querybr.getColumn(0).getBytes();
            item.data.resize(bytes);
            memcpy_s(item.data.data(), bytes, querybr.getColumn(0).getBlob(), bytes);
            return 1; //ֻѡ����ϵĵ�һ��
        }
    }
    return 0;
}

int Cache::clearCacheData()
{
    SQLite::Database& db = *(_impl->db);
    //ɾ�����ڵı�
    db.exec("DROP TABLE IF EXISTS sht_data");
    //���´��������ʹ�õı�
    return db.exec("CREATE TABLE sht_data (id INTEGER PRIMARY KEY, frame_num INTEGER, set_name TEXT, set_type TEXT, data_byte BLOB)");
}

int Cache::countCacheData()
{
    SQLite::Database& db = *(_impl->db);
    SQLite::Statement querybr(db, "SELECT count(*) FROM sht_data");
    while (querybr.executeStep()) {
        if (querybr.getColumn(0).isInteger()) {
            int count = querybr.getColumn(0).getInt();
            return count;
        }
    }
    return 0;
}

} // namespace sht
