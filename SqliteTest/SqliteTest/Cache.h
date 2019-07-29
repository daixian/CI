#pragma once
#include <string>
#include <vector>

namespace sht {

class Cache
{
  public:
    Cache();
    ~Cache();

    static Cache* GetInst()
    {
        if (m_pInstance == nullptr)
            m_pInstance = new Cache();
        return m_pInstance;
    }

    /// <summary> 数据库文件路径(只是一个记录). </summary>
    std::string dbPath;

    /// <summary> 数据库加密的key. </summary>
    std::string key = "12345678";

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 一项纪录. </summary>
    ///
    /// <remarks> Dx, 2019/7/29. </remarks>
    ///-------------------------------------------------------------------------------------------------
    class Item
    {
      public:
        Item()
        {}
        Item(int frameNum, const std::string setName) : frameNum(frameNum), setName(setName)
        {
        }
        Item(int frameNum, const std::string setName, const std::string setType) : frameNum(frameNum), setName(setName), setType(setType)
        {
        }

        int frameNum;
        std::string setName;
        std::string setType;
        std::vector<char> data;
    };

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

#pragma region Cache相关函数

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 添加一项数据. </summary>
    ///
    /// <remarks> Dx, 2019/7/26. </remarks>
    ///
    /// <param name="item"> The frame number. </param>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    int addCacheData(const Item& item);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Gets a data. </summary>
    ///
    /// <remarks> Dx, 2019/7/26. </remarks>
    ///
    /// <param name="item"> [in,out] 先存好需要依据的查询内容,查询补上data. </param>
    ///
    /// <returns> The data. </returns>
    ///-------------------------------------------------------------------------------------------------
    int getCacheData(Item& item);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 清空cache表. </summary>
    ///
    /// <remarks> Dx, 2019/7/27. </remarks>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    int clearCacheData();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 当前一共有多少条CacheData的计数. </summary>
    ///
    /// <remarks> Dx, 2019/7/27. </remarks>
    ///
    /// <returns> The total number of cache data. </returns>
    ///-------------------------------------------------------------------------------------------------
    int countCacheData();

#pragma endregion

  private:
    static Cache* m_pInstance;

    class Impl;
    Impl* _impl;
};

} // namespace sht
