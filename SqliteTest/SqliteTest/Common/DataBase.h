#pragma once

#include <string>
#include <vector>

namespace dxlib {

///-------------------------------------------------------------------------------------------------
/// <summary> һ�����ݿ����ͨ��. </summary>
///
/// <remarks>
/// SQLite��db3�ļ��Ĳ鿴����������Ѿ��滻�˼��ܰ�sqlite3.dll�ļ������������DB
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

    /// <summary> ���ݿ��ļ�·��(ֻ��һ����¼). </summary>
    std::string dbPath;

    /// <summary> ���ݿ���ܵ�key. </summary>
    std::string key = "12345678";

    ///-------------------------------------------------------------------------------------------------
    /// <summary> �Ƿ����ݿ��Ѿ�����. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <returns>
    /// True if database file exists, false if not.
    /// </returns>
    ///-------------------------------------------------------------------------------------------------
    bool isDBOpened();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> �����µ����ݿ�. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="path"> Full pathname of the file. </param>
    ///-------------------------------------------------------------------------------------------------
    int creatNewDB(const std::string& path);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> ��һ�����ݿ�(ֻ��). </summary>
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
    /// <summary> ��һ�����ݿ�(��д). </summary>
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
    /// <summary> �ر����ݿ�. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void closeDB();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> ִ���Լ�д��SQL. </summary>
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