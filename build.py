import urllib
from urllib import request
import os

dirConcur = "./concurrentqueue/"
if not os.path.exists(dirConcur):
    os.makedirs(dirConcur)

print("下载...")
url = 'https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h'
request.urlretrieve(url, "./concurrentqueue/blockingconcurrentqueue.h")
url = "https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h"
request.urlretrieve(url, "./concurrentqueue/concurrentqueue.h")
print("下载完成")