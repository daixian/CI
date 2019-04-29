cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

set(SUB_NAME "test.out")
add_executable(${SUB_NAME} pch.h pch.cpp test.cpp)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# CMake 2.8.11 之后gtest是自动添加的，所以下面的没有执行
if (CMAKE_VERSION  VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

# 设置dlog库的库目录,x64版本
if(WIN32)
    if(NOT DEFINED dlog_LIB_DIR)
        set(dlog_LIB_DIR ${PROJECT_LIB_DIR}/dlog/x64)
    endif()
    set(dlog_LIB ${dlog_LIB_DIR}/dlog.lib)
    #拷贝dlog.dll到安装目录
    # add_custom_command(TARGET app POST_BUILD        # Adds a post-build event to MyTest
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
    #         "${PROJECT_LIB_DIR}/dlog/x64/dlog.dll"      # <--this is in-file
    #         ${CMAKE_INSTALL_PREFIX}/dlog.dll)                 # <--this is out-file path
    
    # 这里的复制没有太好的方法,因为vs会自己多出Release文件夹来,只能把文件夹名字写死了
    # https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake?rq=1
    # https://stackoverflow.com/questions/39282430/how-to-get-current-configuration-release-debug-in-cmake-for-visual-studio
    # add_custom_command(TARGET app POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dlog_LIB_DIR}/dlog.dll" ${EXECUTABLE_OUTPUT_PATH}/Release/dlog.dll)
    # add_custom_command(TARGET app POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dlog_LIB_DIR}/dlog.dll" ${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo/dlog.dll)
else()
    if(NOT DEFINED dlog_LIB_DIR)
        set(dlog_LIB_DIR ${PROJECT_LIB_DIR}/dlog)
    endif()
    set(dlog_LIB ${dlog_LIB_DIR}/libdlog.a)  
    # 其他平台目前是静态编译的，就没有拷贝dll
endif()
if(CROSS_COMPILE)
    set(dlog_LIB ${dlog_LIB_DIR}_arm64/libdlog.a)
endif()
message(STATUS "dlog_LIB = " ${dlog_LIB})

find_package(Boost 1.70.0 REQUIRED COMPONENTS filesystem date_time)
message(STATUS "Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_LIBRARIES = " ${Boost_LIBRARIES})

find_package (Threads)

# Dependencies
target_include_directories(${SUB_NAME} PRIVATE ${CMAKE_SOURCE_DIR} ${PROJECT_LIB_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(${SUB_NAME} PRIVATE ${dlog_LIB} ${Boost_LIBRARIES} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME AllCppTest COMMAND ${SUB_NAME})


# 这是vcpkg时候的写法
# Dependencies
# find_package(GTest MODULE REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# target_link_libraries(utcpp.out PRIVATE dlog GTest::GTest GTest::Main)
#add_test(NAME TestsInUTCpp COMMAND utcpp.out)
