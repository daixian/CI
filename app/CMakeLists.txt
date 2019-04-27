cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

# 隐藏导出函数这个确实是有效的，但是目前看来隐藏它并没有什么意义，会导致测试工程找不到函数还
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_C_VISIBILITY_PRESET hidden)

add_definitions(-D DLOG_EXPORTS)
add_definitions(-D UNICODE)
if(WIN32)
  add_definitions(-D _WINDOWS)
  add_definitions(-D _USERDLL)
endif()

# Introduce variables: * CMAKE_INSTALL_LIBDIR * CMAKE_INSTALL_BINDIR *
# CMAKE_INSTALL_INCLUDEDIR

# Layout. This works for all platforms: * <prefix>/lib*/cmake/<PROJECT-NAME> *
# <prefix>/lib*/ * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

add_executable(app main.cpp)

message(STATUS "CMAKE_CFG_INTDIR = " ${CMAKE_CFG_INTDIR})

# 设置dlog库的库目录,x64版本
if(NOT DEFINED dlog_LIB_DIR)
  set(dlog_LIB_DIR ${PROJECT_LIB_DIR}/dlog/x64)
endif()
set(dlog_LIB ${dlog_LIB_DIR}/dlog.lib)
message(STATUS "dlog_LIB = " ${dlog_LIB})
#拷贝dlog.dll到安装目录
add_custom_command(TARGET app POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_LIB_DIR}/dlog/x64/dlog.dll"      # <--this is in-file
        ${CMAKE_INSTALL_PREFIX}/dlog.dll)                 # <--this is out-file path
 
# 这里的复制没有太好的方法,因为vs会自己多出Release文件夹来,只能把文件夹名字写死了
# https://stackoverflow.com/questions/39282430/how-to-get-current-configuration-release-debug-in-cmake-for-visual-studio
if(WIN32)
add_custom_command(TARGET app POST_BUILD       
COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dlog_LIB_DIR}/dlog.dll" ${EXECUTABLE_OUTPUT_PATH}/Release/dlog.dll)
add_custom_command(TARGET app POST_BUILD       
COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dlog_LIB_DIR}/dlog.dll" ${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo/dlog.dll)  
endif()

# 设置boost库       
find_package(Boost 1.70.0 REQUIRED COMPONENTS filesystem date_time)
message(STATUS "Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_LIBRARIES = " ${Boost_LIBRARIES})

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR} ${PROJECT_LIB_DIR} ${Boost_INCLUDE_DIRS}  )
target_link_libraries(app PRIVATE ${Boost_LIBRARIES} ${dlog_LIB})

# 全都安装到根目录
install(TARGETS app
       RUNTIME DESTINATION "./"
       LIBRARY DESTINATION "./"
       ARCHIVE DESTINATION "./"
)
# 安装头文件
# install(FILES "dlog.h" DESTINATION "./")
# install(FILES "../UnitTest/DLog.cs" DESTINATION "./")

# Configuration
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
