﻿#include "DBHelper.h"
#include <boost/format.hpp>
//#include "dlog/dlog.h"

#pragma comment(lib, "SQLiteCpp.lib")
#pragma comment(lib, "sqlite3.lib")

#define LogI printf
#define LogE printf

namespace dxlib {

using namespace std;

int DBHelper::UPDATE(SQLite::Database* SQLiteCppDB, const std::string& table_name,
                     const std::string& where_col_name, const std::string& where_row_value,
                     const std::string& update_col_name, const std::string& update_value)
{
    if (SQLiteCppDB == nullptr) {
        LogE("DBHelper.UPDATE():传入Database指针为NULL!");
        return 0;
    }
    SQLite::Database& db = *SQLiteCppDB;
    string sql = (boost::format("UPDATE %s SET %s=? WHERE %s='%s'") % table_name %
                  update_col_name %
                  where_col_name % where_row_value)
                     .str();

    SQLite::Statement queryb(db, sql);
    queryb.bind(1, update_value);
    return queryb.exec();
}

int DBHelper::SELECT(SQLite::Database* SQLiteCppDB, const std::string& table_name,
                     const std::string& where_col_name, const std::string& where_row_value,
                     const std::string& select_col_name, std::string& value)
{
    if (SQLiteCppDB == nullptr) {
        LogE("DBHelper.SELECT():传入Database指针为NULL!");
        return 0;
    }
    SQLite::Database& db = *SQLiteCppDB;

    string sql = (boost::format("SELECT %s FROM %s WHERE %s='%s'") % select_col_name % table_name %
                  where_col_name % where_row_value)
                     .str();

    //从数据库读信息
    SQLite::Statement querybr(db, sql);
    while (querybr.executeStep()) {
        if (querybr.getColumn(0).isText()) {
            value = querybr.getColumn(0).getString(); //只选第一条
            return 1;
        }
    }
    return 0;
}

int DBHelper::SELECT(SQLite::Database* SQLiteCppDB, const std::string& table_name,
                     const std::string& where_col_name, const std::string& where_row_value,
                     const std::string& select_col_name, unsigned int& value)
{
    if (SQLiteCppDB == nullptr) {
        LogE("DBHelper.SELECT():传入Database指针为NULL!");
        return 0;
    }
    SQLite::Database& db = *SQLiteCppDB;

    string sql = (boost::format("SELECT %s FROM %s WHERE %s='%s'") % select_col_name % table_name %
                  where_col_name % where_row_value)
                     .str();

    //从数据库读信息
    SQLite::Statement querybr(db, sql);
    while (querybr.executeStep()) {
        if (querybr.getColumn(0).isText()) {
            value = querybr.getColumn(0).getUInt(); //只选第一条
            return 1;
        }
    }
    return 0;
}

int DBHelper::SELECT(SQLite::Database* SQLiteCppDB, const std::string& table_name,
                     const std::string& where_col_name, const std::string& where_row_value,
                     const std::string& select_col_name, std::vector<char>& value)
{
    if (SQLiteCppDB == nullptr) {
        LogE("DBHelper.SELECT():传入Database指针为NULL!");
        return 0;
    }
    SQLite::Database& db = *SQLiteCppDB;

    string sql = (boost::format("SELECT %s FROM %s WHERE %s='%s'") % select_col_name % table_name %
                  where_col_name % where_row_value)
                     .str();

    //从数据库读信息
    SQLite::Statement querybr(db, sql);
    while (querybr.executeStep()) {
        if (querybr.getColumn(0).isBlob()) {
            int bytes = querybr.getColumn(0).getBytes();
            value.resize(bytes);
            memcpy_s(value.data(), bytes, querybr.getColumn(0).getBlob(), bytes);
            return 1;
        }
    }
    return 0;
}

} // namespace dxlib