#include "pch.h"
#include "boost/filesystem.hpp"
#include "dlog/dlog.h"

#if defined(_WIN32) || defined(_WIN64)

#elif defined(__linux__)
#endif

using namespace std;

//注意,在微软的CI服务器上,APPDATA目录没有权限创建,同时不支持中文路径

TEST(app, test)
{
    namespace fs = boost::filesystem;
    fs::create_directories("./123");
    ASSERT_TRUE(true);
}

TEST(app, test2)
{
    dlog_init();
    LogI("21313131212312");
    ASSERT_TRUE(true);
}
