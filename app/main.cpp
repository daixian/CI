#include <cstdio>
#include "dlog/dlog.h"
#include "opencv2/opencv.hpp"

//#pragma execution_character_set("GB2312") 这个头定义不需要,但是要注意定义bom头

int main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    dlog_init("log", "main", dlog_init_relative::MODULE);
    for (size_t i = 0; i < 10; i++) {
        LogI("log %d", i);
        LogI("输出一条日志 %d", i);
    }

#pragma omp parallel sections //并行区域
    {
#pragma omp section //负责这个区域的线程对前面部分进行排序
        while (true) {
        }
#pragma omp section //负责这个区域的线程对后面部分进行排序
        while (true) {
        }
#pragma omp section //负责这个区域的线程对后面部分进行排序
        while (true) {
        }
#pragma omp section //负责这个区域的线程对后面部分进行排序
        while (true) {
        }
#pragma omp section //负责这个区域的线程对后面部分进行排序
        while (true) {
        }
    }
    return 0;
}