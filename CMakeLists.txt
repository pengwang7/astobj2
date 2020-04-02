cmake_minimum_required(VERSION 3.9.6)
project(astobj2)



include_directories(
    /home/pengwang/cloud_platform/project/astobj2/__astobj2
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pg -pipe -Wall -g -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wunused-result")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lglog")

set(BUTIL_SOURCES
    #${PROJECT_SOURCE_DIR}/__astobj2/lock.cpp
    #${PROJECT_SOURCE_DIR}/__astobj2/utils.cpp
    #${PROJECT_SOURCE_DIR}/__astobj2/astmm.cpp
    ${PROJECT_SOURCE_DIR}/__astobj2/astobj2.cpp
)

add_executable(${PROJECT_NAME} ${BUTIL_SOURCES})

#target_link_libraries(dict /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so)
#target_link_libraries(dict /home/pengwang/cloud_platform/target/3rd/glog/libglog.a)
