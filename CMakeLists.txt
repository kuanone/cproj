cmake_minimum_required(VERSION 3.25)

#调用CMake的 project 时定义项目版本号
#project(cproj VERSION 0.0.1 LANGUAGES C)
#set(CMAKE_C_STANDARD 17)

project(cproj VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

ADD_DEFINITIONS(-DJEMALLOC_NO_DEMANGLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -ldl -ljemalloc -g -O3")

#动态库的方式
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -ldl -ljemalloc -g -O3")

#静态库方式
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -ldl -g -O3")

include_directories(
        /usr/local/include
)

link_directories(
        /usr/local/libcd
)

aux_source_directory(. SRC)
set(ALL ${SRC})

#基于 version.h.in 生成 version.h
configure_file(
        version.h.in
#        generated/version.h
        ../version.h
        @ONLY
)

add_subdirectory(3rd/cJSON)

add_executable(cproj main.cpp
        3rd/cJSON/cJSON.c)

#target_include_directories(cproj PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_include_directories(cproj PRIVATE)

target_link_libraries(${PROJECT_NAME} cjson  /usr/local/lib/libjemalloc.a)