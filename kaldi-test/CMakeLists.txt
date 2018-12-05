cmake_minimum_required(VERSION 2.7)


PROJECT(asr-test)

#INCLUDE_DIRECTORIES()        

add_definitions(-std=c++11)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(asr-test ${DIR_SRCS})
TARGET_LINK_LIBRARIES(asr-test pthread)
