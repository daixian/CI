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
    cv::Mat img(600, 800, CV_8UC3, cv::Scalar(0, 255, 0));
    cv::imshow("123", img);
    cv::waitKey();
    return 0;
}