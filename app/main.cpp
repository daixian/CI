#include <cstdio>
#include "dlog/dlog.h"
#include "opencv2/opencv.hpp"
#include "concurrentqueue/concurrentqueue.h"
#include "CoolTime.hpp"

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
    long long count[4] = {0, 0, 0, 0};
    dxlib::CoolTime ct[5];
    for (size_t i = 0; i < 5; i++) {
        ct[i].CD = 10;
    }

    LogI("开始执行并行实验！");
#pragma omp parallel sections //并行区域
    {
#pragma omp section //并行5 （实际实验count的增长是每秒2W8次左右,开始的时候能启动5核，后来只能启动4核）
        while (true) {
            for (size_t i = 0; i < 4; i++) {
                std::string msg;
                if (queue[i].try_dequeue(msg)) {
                    count[i]++;
                    if (ct[4].isDone()) {
                        printf("count ={%lld, %lld, %lld, %lld}\r\n", count[0], count[1], count[2], count[3]);
                    }
                }
            }
        }
#pragma omp section //并行1
        while (true) {
            if (ct[0].isDone()) {
                LogI("并行1 执行...");
            }
            if (queue[0].size_approx() == 0)
                queue[0].enqueue("section0");
        }
#pragma omp section //并行2
        while (true) {
            if (ct[1].isDone()) {
                LogI("并行2 执行...");
            }
            if (queue[1].size_approx() == 0)
                queue[1].enqueue("section1");
        }
#pragma omp section //并行3
        while (true) {
            if (ct[2].isDone()) {
                LogI("并行3 执行...");
            }
            if (queue[2].size_approx() == 0)
                queue[2].enqueue("section2");
        }
#pragma omp section //并行4
        while (true) {
            if (ct[3].isDone()) {
                LogI("并行4 执行...");
            }
            if (queue[3].size_approx() == 0)
                queue[3].enqueue("section3");
        }
    }
    return 0;
}