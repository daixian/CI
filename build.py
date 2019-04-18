# coding: utf-8
import urllib
from urllib import request
import os
import sys
import tarfile
import zipfile
import shutil

# 临时下载文件夹
dirDownload = "./download"
if not os.path.exists(dirDownload):
    os.makedirs(dirDownload)

# 项目的依赖库文件夹
dirLib = "./lib"
if not os.path.exists(dirLib):
    os.makedirs(dirLib)


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
    '''
    function:解压
    params:
        tar_path:压缩文件路径
        target_path:解压缩路径
    description:
    '''
    try:
        tar = tarfile.open(tar_path, "r:gz")
        file_names = tar.getnames()
        for file_name in file_names:
            tar.extract(file_name, target_path)
        tar.close()
    except:
        print(tar_path+"unzip error")


def progress_bar(name, total, progress):
    """
    Displays or updates a console progress bar.
    Original source: https://stackoverflow.com/a/15860757/1391441
    """
    barLength, status = 40, ""
    progress = float(progress) / float(total)
    if progress < 0:
        return
    if progress >= 1.:
        progress, status = 1, ""  # "\r\n"
    block = int(round(barLength * progress))
    text = "\r{} [{}] {:.0f}% {}".format(name,
                                         "#" * block + "-" *
                                         (barLength - block), round(progress * 100, 0),
                                         status)
    sys.stdout.write(text)
    sys.stdout.flush()


# 用来记一个状态,输出日志行数太多
report_lastper = -1


def request_report(bcount, bsize, size):
    '''输出一个下载进度
    params:
        bcount:已下载的块数量
        bsize:块大小
        size:文件总大小
    '''
    if size == -1:  # 可能是不支持进度那么就是-1
        return
    per = 100*bcount*bsize/size
    per = round(per, 2)
    if per > 100:
        per = 100
    global report_lastper
    if per - report_lastper > 1 or per == 100:
        report_lastper = per
        progress_bar("download:", 100, per)


def download_with_cache(in_url, in_filepath):
    '''下载前判断一次是否文件已经缓存了'''
    if os.path.exists(in_filepath) and os.path.isfile(in_filepath):
        # 如果本地存在缓存那么就不下载了
        print(in_filepath+" [cached]")
    else:
        print(in_url+" -> "+in_filepath)
        # 有的系统不能\r,所以就先不输出进度了
        request.urlretrieve(in_url, in_filepath, request_report)
        # request.urlretrieve(in_url, in_filepath)
        sys.stdout.write("\r\n")
        sys.stdout.flush()
    print(in_filepath+" [done]")


def download_concurrentqueue():
    '''下载库 concurrentqueue'''
    dirConcur = dirLib+"/concurrentqueue"
    if not os.path.exists(dirConcur):
        os.makedirs(dirConcur)

    print("download concurrentqueue ...")
    url = 'https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h'
    download_with_cache(url, dirConcur + "/blockingconcurrentqueue.h")
    url = "https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h"
    download_with_cache(url, dirConcur + "/concurrentqueue.h")
    print("done!")


def download_spdlog():
    '''下载库 spdlog'''
    print("download spdlog ...")
    url = "https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"
    downloadFile = dirDownload + "/spdlog-v1.3.1.tar.gz"
    download_with_cache(url, downloadFile)

    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/spdlog"):
        shutil.rmtree(dirLib+"/spdlog")
    os.renames(dirLib + "/spdlog-1.3.1", dirLib + "/spdlog")
    print("done!")


def download_gtest():
    '''下载库 gtest'''
    print("download gtest ...")
    url = "https://github.com/google/googletest/archive/release-1.8.1.tar.gz"
    downloadFile = dirDownload + "/gtest-1.8.1.tar.gz"
    download_with_cache(url, downloadFile)

    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/gtest"):
        shutil.rmtree(dirLib+"/gtest")
    os.renames(dirLib+"/googletest-release-1.8.1", dirLib+"/gtest")
    print("done!")


def download_cryptopp():
    '''下载库 cryptopp'''
    print("download cryptopp ...")
    url = "https://github.com/weidai11/cryptopp/archive/CRYPTOPP_8_1_0.tar.gz"
    downloadFile = dirDownload + "/CRYPTOPP_8_1_0.tar.gz"
    download_with_cache(url, downloadFile)

    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/cryptopp"):
        shutil.rmtree(dirLib+"/cryptopp")
    os.renames(dirLib+"/cryptopp-CRYPTOPP_8_1_0", dirLib+"/cryptopp")
    # 到这里只是下载了头文件
    url = "http://mr.xuexuesoft.com:8010/build/cryptopp/x64/Release/MD/cryptlib.zip"
    downloadFile = dirDownload + "/cryptlib-x64-release-md.zip"
    download_with_cache(url, downloadFile)
    extract_zip(downloadFile, dirLib + "/cryptopp/x64/Release/MD")
    print("done!")


def download_dlog():
    '''下载库 dlog'''
    print("download dlog ...")
    url = "https://github.com/daixian/dlog/releases/download/v2.3.0/Release-Windows.zip"
    downloadFile = dirDownload + "/dlog.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib+"/dlog"):
        shutil.rmtree(dirLib + "/dlog")
    extract_zip(downloadFile, dirLib)

    shutil.rmtree(dirLib+"/dlog/x86")
    os.renames(dirLib + "/dlog/x64/dlog.h", dirLib + "/dlog/dlog.h")
    print("done!")


def download_boost():
    '''下载库 boost'''
    print("download boost ...")
    url = "http://mr.xuexuesoft.com:8010/build/boost_1_70_0.zip"
    downloadFile = dirDownload + "/boost_1_70_0.zip"
    download_with_cache(url, downloadFile)

    # if os.path.exists(dirLib+"/boost_1_70_0"):
    #     shutil.rmtree(dirLib + "/boost_1_70_0")
    print("unzip boost ...")
    extract_zip(downloadFile, dirLib)
    print("done!")


def download_eigen():
    '''下载库 eigen'''
    print("download eigen ...")
    url = "https://github.com/eigenteam/eigen-git-mirror/archive/3.3.7.tar.gz"
    downloadFile = dirDownload + "/eigen-3.3.7.tar.gz"
    download_with_cache(url, downloadFile)

    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/eigen"):
        shutil.rmtree(dirLib+"/eigen")
    os.renames(dirLib+"/eigen-git-mirror-3.3.7", dirLib+"/eigen")
    print("done!")


def download_eventbus():
    '''下载库 eventbus'''
    print("download eventbus ...")
    url = "https://github.com/gelldur/EventBus/archive/v2.4.1.tar.gz"
    downloadFile = dirDownload + "/eventbus-v2.4.1.tar.gz"
    download_with_cache(url, downloadFile)

    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/EventBus"):
        shutil.rmtree(dirLib+"/EventBus")
    os.renames(dirLib+"/EventBus-2.4.1", dirLib+"/EventBus")
    print("done!")


download_concurrentqueue()
download_spdlog()
download_gtest()
download_cryptopp()
download_dlog()
download_boost()
download_eigen()
download_eventbus()
