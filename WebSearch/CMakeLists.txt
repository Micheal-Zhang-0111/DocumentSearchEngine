cmake_minimum_required(VERSION 3.0)

project(WebSearch)

include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置可执行文件生成到哪里
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 设置 CMAKE_CXX_STANDARD 变量为 17
set(CMAKE_CXX_STANDARD 17)

# add_executable(main_exe main.cpp src/Gun.cpp src/Soldier.cpp)
file(GLOB SOURCES "src/*.cpp")
add_executable(WebSearch ${SOURCES})

# 链接 pthread 库
target_link_libraries(WebSearch 
    log4cpp
    redis++
    hiredis
    pthread
)