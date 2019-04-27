#include <stdio.h>
#include "dlog/dlog.h"

//#pragma execution_character_set("GB2312") 这个头定义不需要,但是要注意定义bom头

int main(int argc, char const *argv[])
{
    dlog_init("log", "main", dlog_init_relative::MODULE);
    for (size_t i = 0; i < 10; i++) {
        LogI("log %d", i);
        LogI("输出一条日志 %d", i);
    }

    return 0;
}
