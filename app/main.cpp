#include <cstdio>
#include "dlog/dlog.h"
#include "opencv2/opencv.hpp"
#include "concurrentqueue/concurrentqueue.h"

//#pragma execution_character_set("GB2312") 这个头定义不需要,但是要注意定义bom头

int main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    dlog_init("log", "main", dlog_init_relative::MODULE);
    for (size_t i = 0; i < 10; i++) {
        LogI("log %d", i);
        LogI("输出一条日志 %d", i);
    }

    moodycamel::ConcurrentQueue<std::string> queue[4];
    long long count = 0;
#pragma omp parallel sections //并行区域
    {
#pragma omp section //并行1
        while (true) {
            if (queue[0].size_approx() == 0)
                queue[0].enqueue("section0");
        }
#pragma omp section //并行2
        while (true) {
            if (queue[1].size_approx() == 0)
                queue[1].enqueue("section1");
        }
#pragma omp section //并行3
        while (true) {
            if (queue[2].size_approx() == 0)
                queue[2].enqueue("section2");
        }
#pragma omp section //并行4
        while (true) {
            if (queue[3].size_approx() == 0)
                queue[3].enqueue("section3");
        }
#pragma omp section //并行5 （实际实验count的增长是每秒100W次左右）
        while (true) {
            for (size_t i = 0; i < 4; i++) {
                std::string msg;
                if (queue[i].try_dequeue(msg)) {
                    count++;
                    if (count % 1000000 == 0) {
                        printf("count = %lld\r\n", count);
                    }
                }
                //printf("%s\r\n",msg.c_str());
            }
        }
    }
    return 0;
}