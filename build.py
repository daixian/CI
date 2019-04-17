# coding: utf-8
import urllib
from urllib import request
import os
import tarfile
import zipfile
import shutil

# 下载concurrentqueue
dirConcur = "./concurrentqueue"
if not os.path.exists(dirConcur):
    os.makedirs(dirConcur)

print("download concurrentqueue ...")
url = 'https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h'
request.urlretrieve(url, dirConcur + "/blockingconcurrentqueue.h")
url = "https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h"
request.urlretrieve(url, dirConcur + "/concurrentqueue.h")
print("done!")

# 临时下载文件夹
dirDownload = "./download"
if not os.path.exists(dirDownload):
    os.makedirs(dirDownload)


def extract_zip(zfile_path, unzip_dir):
    '''
    function:解压
    params:
        zfile_path:压缩文件路径
        unzip_dir:解压缩路径
    description:
    '''
    try:
        with zipfile.ZipFile(zfile_path) as zfile:
            zfile.extractall(path=unzip_dir)
    except zipfile.BadZipFile as e:
        print(zfile_path+"unzip error"+e)


def extract_tar(tar_path, target_path):
    try:
        tar = tarfile.open(tar_path, "r:gz")
        file_names = tar.getnames()
        for file_name in file_names:
            tar.extract(file_name, target_path)
        tar.close()
    except:
        print(tar_path+"unzip error")


# 下载 spdlog
print("download spdlog ...")
url = "https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"
downloadFile = dirDownload + "/v1.3.1.tar.gz"
request.urlretrieve(url, downloadFile)
print("done!")

extract_tar(downloadFile, "./")

if os.path.exists("./spdlog"):
    shutil.rmtree("./spdlog")

os.renames("./spdlog-1.3.1", "./spdlog")

# 下载 gtest
print("download gtest ...")
url = "https://github.com/google/googletest/archive/release-1.8.1.tar.gz"
downloadFile = dirDownload + "/release-1.8.1.tar.gz"
request.urlretrieve(url, downloadFile)
print("done!")

extract_tar(downloadFile, "./")

if os.path.exists("./gtest"):
    shutil.rmtree("./gtest")

os.renames("./googletest-release-1.8.1", "./gtest")

# 下载 dlog
print("download dlog ...")
url = "https://github.com/daixian/dlog/releases/download/v2.3.0/Release-Windows.zip"
downloadFile = dirDownload + "/dlog.zip"
request.urlretrieve(url, downloadFile)
print("done!")

extract_zip(downloadFile, "./")
shutil.rmtree("./dlog/x86")
os.renames("./dlog/x64/dlog.h", "./dlog/dlog.h")
