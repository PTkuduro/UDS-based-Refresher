cmake_minimum_required(VERSION 2.8)

project(diag)
include_directories(
    ${PROJECT_SOURCE_DIR}/include/
)
FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(headers SOURCES ${LibFiles})
set(DIR_SRCS src)
set(SRC
    ${DIR_SRCS}/main.cpp
    ${DIR_SRCS}/UDS.cpp
    ${DIR_SRCS}/socket_can.cpp
    ${DIR_SRCS}/s19_FileRead.cpp
    ${DIR_SRCS}/global.cpp
    ${DIR_SRCS}/SA_Algorithm.cpp


)
set(CMAKE_CXX_FLAGS "-Wall -pthread -fpermissive -std=c++11  -lpthread -O2")

add_executable(${PROJECT_NAME} ${SRC})
