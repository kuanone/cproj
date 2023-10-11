cmake_minimum_required(VERSION 3.25)

#调用CMake的 project 时定义项目版本号
project(cproj VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

#基于 version.h.in 生成 version.h
configure_file(
        version.h.in
#        generated/version.h
        ../version.h
        @ONLY
)

add_subdirectory(3rd/cJSON)

add_executable(cproj main.c
        3rd/cJSON/cJSON.c)

#target_include_directories(cproj PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_include_directories(cproj PRIVATE)

target_link_libraries(${PROJECT_NAME} cjson)