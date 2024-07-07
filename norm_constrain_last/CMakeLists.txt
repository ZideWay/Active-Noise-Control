cmake_minimum_required(VERSION 3.20.0)
project(anc)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 搜索源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# 检查是否找到源文件
if(NOT SRC_LIST)
    message(FATAL_ERROR "No source files found in ${PROJECT_SOURCE_DIR}/src")
endif()

# 创建共享库
add_library(common SHARED ${SRC_LIST})

# 设置头文件目录
target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(main ${PROJECT_SOURCE_DIR}/src/main.c)

# 设置可执行文件头文件目录
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 链接库文件
target_link_libraries(main PUBLIC common)
