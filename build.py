# coding: utf-8
import urllib
from urllib import request
import os
import tarfile

#下载concurrentqueue
dirConcur = "./concurrentqueue"
if not os.path.exists(dirConcur):
    os.makedirs(dirConcur)

print("download concurrentqueue ...")
url = 'https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h'
request.urlretrieve(url, dirConcur + "/blockingconcurrentqueue.h")
url = "https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h"
request.urlretrieve(url, dirConcur + "/concurrentqueue.h")
print("done!")

#临时下载文件夹
dirDownload = "./download"
if not os.path.exists(dirDownload):
    os.makedirs(dirDownload)

print("download spdlog ...")
url = "https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"
request.urlretrieve(url, dirDownload + "/v1.3.1.tar.gz")
print(""done!")