// SqliteTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Cache.h"
#include <boost/filesystem.hpp>

int main()
{
    using namespace sht;
    using namespace std;
    namespace fs = boost::filesystem;

    vector<char> data;
    for (size_t i = 0; i < 1024; i++) {
        data.push_back((char)i);
    }
    fs::path dbPath = "cache.db3";
    if (fs::is_regular_file(dbPath)) {
        fs::remove(dbPath); //如果文件存在了就删除文件,免得影响
    }

    //创建一个新数据库
    int res = Cache::GetInst()->creatNewDB(dbPath.string());

    //写一个123命名的数据
    res = Cache::GetInst()->addCacheData("123", data);

    //读这个123命名的数据
    vector<char> data2;
    res = Cache::GetInst()->getCacheData("123", data2);

    res = Cache::GetInst()->clearCacheData();

    res = Cache::GetInst()->getCacheData("123", data2);

    clock_t t0 = clock();
    for (size_t i = 0; i < 1024 * 10; i++) {
        res = Cache::GetInst()->addCacheData("123", data);
    }
    printf("写1024*10次,耗时%f秒", (float)(clock() - t0) / CLOCKS_PER_SEC);
    getchar();

    //批量提交的方式看速度,即把1024条一批
    t0 = clock(); //重新计时
    Cache::GetInst()->exec("begin;");
    for (size_t i = 0; i < 1024 * 10; i++) {
        res = Cache::GetInst()->addCacheData("123", data);
    }
    Cache::GetInst()->exec("commit;");
    printf("(批量)写1024*10次,耗时%f秒", (float)(clock() - t0) / CLOCKS_PER_SEC);
    getchar();

    int count = Cache::GetInst()->countCacheData();
    printf("当前数据库里有%d条记录", count);
    getchar();
    Cache::GetInst()->closeDB();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
