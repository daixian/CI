#pragma once

#include <string>
#include <vector>

namespace dxlib {

///-------------------------------------------------------------------------------------------------
/// <summary> 一个数据库的普通类. </summary>
///
/// <remarks>
/// SQLite的db3文件的查看工具软件，已经替换了加密版sqlite3.dll文件，运行里面的DB
/// Browser for SQLCipher.exe
/// http://xuexuesoft.com/files/soft/DB%20Browser%20for%20SQLite.zip
/// 
/// Dx, 2019/7/27.
/// </remarks>
///-------------------------------------------------------------------------------------------------
class DataBase
{
  public:
    DataBase();
    ~DataBase();

    /// <summary> 数据库文件路径(只是一个记录). </summary>
    std::string dbPath;

    /// <summary> 数据库加密的key. </summary>
    std::string key = "12345678";

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 是否数据库已经打开了. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <returns>
    /// True if database file exists, false if not.
    /// </returns>
    ///-------------------------------------------------------------------------------------------------
    bool isDBOpened();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 创建新的数据库. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="path"> Full pathname of the file. </param>
    ///-------------------------------------------------------------------------------------------------
    int creatNewDB(const std::string& path);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 打开一个数据库(只读). </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="path"> Full pathname of the file. </param>
    ///
    /// <returns>
    /// True if it succeeds, false if it fails.
    /// </returns>
    ///-------------------------------------------------------------------------------------------------
    bool openDB(const std::string& path);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 打开一个数据库(读写). </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="path"> Full pathname of the file. </param>
    ///
    /// <returns>
    /// True if it succeeds, false if it fails.
    /// </returns>
    ///-------------------------------------------------------------------------------------------------
    bool openDB_RW(const std::string& path);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 关闭数据库. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void closeDB();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 执行自己写的SQL. </summary>
    ///
    /// <remarks> Dx, 2019/7/27. </remarks>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    int exec(const std::string& sql);

  private:
    class Impl;
    Impl* _impl;
};
} // namespace dxlib