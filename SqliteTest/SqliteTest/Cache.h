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

    /// <summary> ���ݿ��ļ�·��(ֻ��һ����¼). </summary>
    std::string dbPath;

    /// <summary> ���ݿ���ܵ�key. </summary>
    std::string key = "12345678";

    ///-------------------------------------------------------------------------------------------------
    /// <summary> һ���¼. </summary>
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

#pragma region Cache��غ���

    ///-------------------------------------------------------------------------------------------------
    /// <summary> ���һ������. </summary>
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
    /// <param name="item"> [in,out] �ȴ����Ҫ���ݵĲ�ѯ����,��ѯ����data. </param>
    ///
    /// <returns> The data. </returns>
    ///-------------------------------------------------------------------------------------------------
    int getCacheData(Item& item);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> ���cache��. </summary>
    ///
    /// <remarks> Dx, 2019/7/27. </remarks>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    int clearCacheData();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> ��ǰһ���ж�����CacheData�ļ���. </summary>
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
