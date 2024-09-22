cmake_minimum_required(VERSION 3.0)

project(SE_Server)

include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置可执行文件生成到哪里
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 设置 CMAKE_CXX_STANDARD 变量为 14
set(CMAKE_CXX_STANDARD 14)

# add_executable(main_exe main.cpp src/Gun.cpp src/Soldier.cpp)
file(GLOB SOURCES "src/SearchEngineCpp/*.cpp" "src/ThreadPool/*.cpp")
add_executable(server ${SOURCES})

#为整个项目添加定义
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)


# 链接 pthread 库
target_link_libraries(server 
    redis++
    hiredis
    log4cpp
    pthread
)