#include "pch.h"

#if defined(_WIN32) || defined(_WIN64)

#elif defined(__linux__)
#endif

using namespace std;

//注意,在微软的CI服务器上,APPDATA目录没有权限创建,同时不支持中文路径

TEST(app, test)
{
    ASSERT_TRUE(true);
}
