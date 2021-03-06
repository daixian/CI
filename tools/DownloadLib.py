# coding: utf-8
import urllib
from urllib import request
import os
import sys
import tarfile
import zipfile
import shutil
import getopt

# 执行的平台
platform = "windows"

# 临时下载文件夹
dirDownload = "./download"

# 工具文件夹
dirTools = "./tools"

# 项目的依赖库文件夹
dirLib = "./lib"

# 项目内部的依赖库文件夹(要使用cmake一起构建的库)
dirInternalLib = "./lib"

# 7zip的工具地址
path7zip = "./tools/7-Zip/7z.exe"


def extract_7z(zfile_path, unzip_dir):
    '''
    function:使用7z工具解压
    params:
        zfile_path:压缩文件路径
        unzip_dir:解压缩文件夹路径
    description:
    '''
    global platform
    global path7zip

    if(platform == "windows"):
        path7zip = os.path.join(dirTools, "7-Zip", "7z.exe")

    # 如果7zip工具不存在那么就下载
    if not os.path.isfile(path7zip):
        download_7zip()
    cmd = '{}  x "{}" -o"{}"'.format(path7zip, zfile_path, unzip_dir)
    os.system(cmd)


def extract_zip(zfile_path, unzip_dir):
    '''
    function:解压
    params:
        zfile_path:压缩文件路径
        unzip_dir:解压缩路径
    description:
    '''
    try:
        if not os.path.exists(unzip_dir):
            os.makedirs(unzip_dir)
        # with zipfile.ZipFile(zfile_path) as zfile:
        #     zfile.extractall(path=unzip_dir) 直接all会中文乱码
        with zipfile.ZipFile(zfile_path, 'r') as zf:
            for fn in zf.namelist():
                right_fn = os.path.join(unzip_dir, fn.encode(
                    'cp437').decode('gbk'))  # 将文件名正确编码
                if (right_fn.endswith('/')):
                    if os.path.exists(right_fn):
                        shutil.rmtree(right_fn)
                    os.makedirs(right_fn)
                else:
                    with open(right_fn, 'wb') as output_file:  # 创建并打开新文件
                        with zf.open(fn, 'r') as origin_file:  # 打开原文件
                            shutil.copyfileobj(
                                origin_file, output_file)  # 将原文件内容复制到新文件
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


# 用来记一个状态,输出日志行数太多
report_lastper = -1


def progress_bar(name, total, progress):
    # 如果已经结束过了,那么就不画进度了
    if report_lastper == total:
        return

    barLength, status = 40, ""
    progress = float(progress) / float(total)
    if progress < 0:
        return
    if progress >= 1.:
        progress, status = 1, "\r\n"  # "\r\n"
    block = int(round(barLength * progress))
    text = "\r{} [{}] {:.2f}% {}".format(name,
                                         "#" * block + "-" *
                                         (barLength - block), round(progress * 100, 2),
                                         status)
    sys.stdout.write(text)
    sys.stdout.flush()


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
        progress_bar("download:", 100, per)
        report_lastper = per


def download_with_cache(in_url, in_filepath):
    '''下载前判断一次是否文件已经缓存了'''
    if os.path.exists(in_filepath) and os.path.isfile(in_filepath):
        # 如果本地存在缓存那么就不下载了
        print(in_filepath+" [cached]")
    else:
        print(in_url+" -> "+in_filepath)
        # 有的系统不能\r,所以就先不输出进度了
        global report_lastper
        report_lastper = -1
        request.urlretrieve(in_url, in_filepath, request_report)
        # request.urlretrieve(in_url, in_filepath)
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
    print("done!\r\n")


def download_spdlog():
    '''下载库 spdlog'''
    print("download spdlog ...")
    url = "https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"
    downloadFile = dirDownload + "/spdlog-v1.3.1.tar.gz"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/spdlog"):
        shutil.rmtree(dirLib+"/spdlog")
    os.renames(dirLib + "/spdlog-1.3.1", dirLib + "/spdlog")
    print("done!\r\n")


def download_gtest():
    '''下载库 gtest'''
    print("download gtest ...")
    url = "https://github.com/google/googletest/archive/release-1.8.1.tar.gz"
    downloadFile = dirDownload + "/gtest-1.8.1.tar.gz"
    download_with_cache(url, downloadFile)

    if not os.path.exists(dirInternalLib):
        os.makedirs(dirInternalLib)

    print("extract start ...")
    extract_tar(downloadFile, dirInternalLib)
    if os.path.exists(dirInternalLib+"/gtest"):
        shutil.rmtree(dirInternalLib+"/gtest")
    os.renames(dirInternalLib+"/googletest-release-1.8.1",
               dirInternalLib+"/gtest")
    print("done!\r\n")


def download_cryptopp():
    '''下载库 cryptopp'''
    print("download cryptopp ...")
    url = "https://github.com/weidai11/cryptopp/archive/CRYPTOPP_8_1_0.tar.gz"
    downloadFile = dirDownload + "/CRYPTOPP_8_1_0.tar.gz"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/cryptopp"):
        shutil.rmtree(dirLib+"/cryptopp")
    os.renames(dirLib+"/cryptopp-CRYPTOPP_8_1_0", dirLib+"/cryptopp")
    # 到这里只是下载了头文件
    # url = "http://mr.xuexuesoft.com:8010/build/cryptopp/x64/Release/MD/cryptlib.zip"
    url = "http://xuexuesoft.com/files/build/cryptlib/MD/cryptlib.zip"
    downloadFile = dirDownload + "/cryptlib-x64-release-md.zip"
    download_with_cache(url, downloadFile)
    extract_zip(downloadFile, dirLib + "/cryptopp/x64/Release/MD")
    print("done!\r\n")


def download_dlog():
    '''下载库 dlog'''
    print("download dlog ...")
    url = "https://github.com/daixian/dlog/releases/download/v2.4.0/Release-Windows.zip"
    downloadFile = dirDownload + "/dlog_2_4_0.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/dlog"):
        shutil.rmtree(dirLib + "/dlog")
    extract_zip(downloadFile, dirLib)

    # shutil.rmtree(dirLib+"/dlog/x86")
    # os.renames(dirLib + "/dlog/x64/dlog.h", dirLib + "/dlog/dlog.h")
    print("done!\r\n")


def download_dlog_linux():
    '''下载库 dlog'''
    print("download dlog ...")
    url = "http://xuexuesoft.com/files/build/linux/x64/dlog.zip"
    downloadFile = dirDownload + "/dlog.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/dlog"):
        shutil.rmtree(dirLib + "/dlog")
    extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_dlog_arm():
    '''下载库 dlog'''
    print("download dlog ...")
    url = "http://xuexuesoft.com/files/build/linux/arm/dlog_arm64.zip"
    downloadFile = dirDownload + "/dlog_arm64.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/dlog_arm64"):
        shutil.rmtree(dirLib + "/dlog_arm64")
    extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_7zip():
    '''下载工具 7zip'''
    print("download 7zip ...")
    url = "https://github.com/daixian/InnoPack/raw/master/assets/files/soft/7-Zip.zip"
    downloadFile = dirDownload + "/7-Zip.zip"
    download_with_cache(url, downloadFile)

    if not os.path.exists(dirTools):
        os.makedirs(dirTools)
    if os.path.exists(dirTools + "/7-Zip"):
        shutil.rmtree(dirTools + "/7-Zip")
    else:
        print("extract start ...")
        extract_zip(downloadFile, dirTools)
    print("done!\r\n")


def download_boost():
    '''下载库 boost'''
    print("download boost ...")
    # url = "http://mr.xuexuesoft.com:8010/build/boost_1_70_0.zip"
    url = "http://xuexuesoft.com/files/build/boost_1_70_0.7z"
    downloadFile = dirDownload + "/boost_1_70_0.7z"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/boost_1_70_0"):
        print("dir exists,don't extract!")
    else:
        # shutil.rmtree(dirLib + "/boost_1_70_0")
        print("extract start ...")
        extract_7z(downloadFile, dirLib)
    print("done!\r\n")


def download_boost_linux():
    '''下载库 boost'''
    print("download boost linux ...")
    url = "http://xuexuesoft.com/files/build/linux/x64/boost_1_70_0.zip"
    downloadFile = dirDownload + "/boost_1_70_0.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/boost_1_70_0"):
        print("dir exists,don't extract!")
    else:
        # shutil.rmtree(dirLib + "/boost_1_70_0")
        print("extract start ...")
        extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_boost_arm():
    '''下载库 boost'''
    print("download boost linux ...")
    url = "http://xuexuesoft.com/files/build/linux/arm/boost_1_70_0_arm64.zip"
    downloadFile = dirDownload + "/boost_1_70_0_arm64.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/boost_1_70_0_arm64"):
        print("dir exists,don't extract!")
    else:
        # shutil.rmtree(dirLib + "/boost_1_70_0")
        print("extract start ...")
        extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_eigen():
    '''下载库 eigen'''
    print("download eigen ...")
    url = "https://github.com/eigenteam/eigen-git-mirror/archive/3.3.7.tar.gz"
    downloadFile = dirDownload + "/eigen-3.3.7.tar.gz"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/eigen"):
        shutil.rmtree(dirLib+"/eigen")
    os.renames(dirLib+"/eigen-git-mirror-3.3.7", dirLib+"/eigen")
    print("done!\r\n")


def download_eventbus():
    '''下载库 eventbus'''
    print("download eventbus ...")
    url = "https://github.com/gelldur/EventBus/archive/v2.4.1.tar.gz"
    downloadFile = dirDownload + "/eventbus-v2.4.1.tar.gz"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    extract_tar(downloadFile, dirLib)
    if os.path.exists(dirLib+"/EventBus"):
        shutil.rmtree(dirLib+"/EventBus")
    os.renames(dirLib+"/EventBus-2.4.1", dirLib+"/EventBus")
    print("done!\r\n")


def download_rapidjson():
    '''下载库 rapidjson'''
    print("download rapidjson ...")
    url = "http://xuexuesoft.com/files/build/rapidjson-master.zip"
    downloadFile = dirDownload + "/rapidjson-190419.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    extract_zip(downloadFile, dirLib)
    if os.path.exists(dirLib+"/rapidjson"):
        shutil.rmtree(dirLib+"/rapidjson")
    os.renames(dirLib+"/rapidjson-master", dirLib+"/rapidjson")
    print("done!\r\n")


def download_fscore():
    '''下载库 fscore'''
    print("download fscore ...")
    url = "http://mr.xuexuesoft.com:8010/build/FSCore_190419.zip"
    downloadFile = dirDownload + "/FSCore_190419.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/FSCore"):
        shutil.rmtree(dirLib+"/FSCore")
    extract_zip(downloadFile, dirLib)

    print("done!\r\n")


def download_rclapi():
    '''下载库 rclapi'''
    print("download rclapi ...")
    url = "http://mr.xuexuesoft.com:8010/build/RclAPI_190419.zip"
    downloadFile = dirDownload + "/RclAPI_190419.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/RclAPI"):
        shutil.rmtree(dirLib+"/RclAPI")
    extract_zip(downloadFile, dirLib)

    print("done!\r\n")


def download_sqlitecpp():
    '''下载库 sqlitecpp'''
    print("download sqlitecpp ...")
    url = "http://mr.xuexuesoft.com:8010/build/SQLiteCpp_190501.zip"
    downloadFile = dirDownload + "/SQLiteCpp_190501.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/SQLiteCpp"):
        shutil.rmtree(dirLib+"/SQLiteCpp")
    extract_zip(downloadFile, dirLib)

    print("done!\r\n")


def download_xuexueutility():
    '''下载库 xuexue.utility'''
    print("download xuexue.utility ...")
    url = "https://github.com/daixian/xuexue.utility/releases/download/V1.0.0/Release-20190323.3.zip"
    downloadFile = dirDownload + "/xuexue.utility_20190323.3.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/xuexue.utility"):
        shutil.rmtree(dirLib + "/xuexue.utility")
    if os.path.exists(dirLib+"/xuexue.unity.utility"):
        shutil.rmtree(dirLib+"/xuexue.unity.utility")
    extract_zip(downloadFile, dirLib)
    os.renames(dirLib + "/utility/xuexue.utility", dirLib + "/xuexue.utility")
    os.renames(dirLib+"/utility/xuexue.unity.utility",
               dirLib+"/xuexue.unity.utility")
    print("done!\r\n")


def download_dotnet_utility():
    '''下载库 dotnet utility'''
    print("download dotnet_utility ...")
    url = "http://mr.xuexuesoft.com:8010/build/dotnet_190419.zip"
    downloadFile = dirDownload + "/dotnet_190419.zip"
    download_with_cache(url, downloadFile)

    print("extract start ...")
    if os.path.exists(dirLib+"/dotnet"):
        shutil.rmtree(dirLib + "/dotnet")
    extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_opencv3():
    '''下载库 opencv3'''
    print("download opencv3 ...")
    url = "http://xuexuesoft.com/files/build/opencv-341-build-x64-vs2017.zip"
    downloadFile = dirDownload + "/opencv-341-build-x64-vs2017.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/opencv-341-build-x64-vs2017"):
        print("dir exists,don't extract!")
    else:
        # shutil.rmtree(dirLib + "/boost_1_70_0")
        print("extract start ...")
        extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_opencv3_linux():
    '''下载库 opencv3 linux'''
    print("download opencv3 linux ...")
    url = "http://xuexuesoft.com/files/build/linux/x64/opencv-3.4.6.zip"
    downloadFile = dirDownload + "/opencv-3.4.6.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/opencv-3.4.6"):
        print("dir exists,don't extract!")
    else:
        print("extract start ...")
        extract_zip(downloadFile, dirLib)
    print("done!\r\n")


def download_opencv3_arm():
    '''下载库 opencv3 arm'''
    print("download opencv3 arm ...")
    url = "http://xuexuesoft.com/files/build/linux/arm/opencv-3.4.6_arm64.zip"
    downloadFile = dirDownload + "/opencv-3.4.6_arm64.zip"
    download_with_cache(url, downloadFile)

    if os.path.exists(dirLib + "/opencv-3.4.6_arm64"):
        print("dir exists,don't extract!")
    else:
        print("extract start ...")
        extract_zip(downloadFile, dirLib)
    print("done!\r\n")

# download_concurrentqueue()
# download_spdlog()
# download_gtest()
# download_cryptopp()
# download_dlog()
# download_boost()
# download_eigen()
# download_eventbus()
# download_rapidjson()
# download_fscore()
# download_rclapi()

# download_xuexueutility()
# download_dotnet_utility()

# 包含目录
# ../lib
# ../lib/rapidjson/include
# ../lib;../lib/rapidjson/include;

# 库目录
# ../lib/dlog/x64
# ../lib/FSCore/x64
# ../lib/RclAPI/x64
# ../lib/FSCore/x64;../lib/dlog/x64;../lib/RclAPI/x64;


def main(argv):
    global platform
    global dirDownload
    global dirLib
    global dirTools
    try:
        # http://www.runoob.com/python/python-command-line-arguments.html?tdsourcetag=s_pcqq_aiomsg
        # 返回值由两个元素组成：第一个是（选项，值）对的列表;第二个是剥离选项列表后留下的程序参数列表（这是第一个参数的尾部切片）。
        # 返回的每个选项和值对都有选项作为其第一个元素，前缀为连字符（例如，' -  x'），
        # 选项参数作为其第二个元素，如果选项没有参数，则为空字符串。选项以与查找顺序相同的顺序出现在列表中，从而允许多次出现。多头和空头选择可能是混合的。
        opts, args = getopt.getopt(
            argv, "hp:d:l:", ["help", "platform=", "download=", "lib="])
    except getopt.GetoptError:
        print("FileTree.py -p <platform>")
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print("FileTree.py -p <platform>")
            sys.exit()
        elif opt in ("-p", "--platform"):
            platform = arg
        elif opt in ("-d", "--download"):
            dirDownload = arg
            father_path = os.path.abspath(dirDownload + os.path.sep + "..")
            dirTools = os.path.join(father_path, "tools")
        elif opt in ("-l", "--lib"):
            dirLib = arg

    print("download lib: platform=" + platform +
          " dirDownload=" + dirDownload+" dirLib=" + dirLib+" dirTools=" + dirTools)
    if not os.path.exists(dirDownload):
        os.makedirs(dirDownload)
    if not os.path.exists(dirLib):
        os.makedirs(dirLib)

    for arg in args:
        if (arg == "concurrentqueue"):
            download_concurrentqueue()

        elif (arg == "spdlog"):
            download_spdlog()

        elif (arg == "gtest"):
            download_gtest()

        elif (arg == "cryptopp"):
            download_cryptopp()

        elif (arg == "dlog"):
            if (platform == "windows"):
                download_dlog()
            elif (platform == "linux"):
                download_dlog_linux()
            elif (platform == "arm"):
                download_dlog_arm()

        elif (arg == "boost"):
            if (platform == "windows"):
                download_boost()
            elif (platform == "linux"):
                download_boost_linux()
            elif (platform == "arm"):
                download_boost_arm()

        elif (arg == "eigen"):
            download_eigen()

        elif (arg == "eventbus"):
            download_eventbus()

        elif (arg == "rapidjson"):
            download_rapidjson()

        elif (arg == "fscore"):
            download_fscore()

        elif (arg == "rclapi"):
            download_rclapi()

        elif (arg == "sqlitecpp"):
            download_sqlitecpp()

        elif (arg == "xuexueutility"):
            download_xuexueutility()

        elif (arg == "dotnet_utility"):
            download_dotnet_utility()

        elif (arg == "opencv3"):
            if (platform == "windows"):
                download_opencv3()
            elif (platform == "linux"):
                download_opencv3_linux()
            elif (platform == "arm"):
                download_opencv3_arm


if __name__ == "__main__":
    main(sys.argv[1:])
