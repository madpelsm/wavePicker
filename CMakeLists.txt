cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(WaveTheoryPicker)
#if (CMAKE_VERSION VERSION_GREATER 3)
#cmake_policy(SET CMP0054 NEW)
#endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules/
})

message(${CMAKE_MODULE_PATH})

include_directories(${PROJECT_SOURCE_DIR})


file(GLOB WTP_SRC_FILES
    src/*.cpp
    dep/lodepng/*.cpp )

ADD_EXECUTABLE(WaveTheoryPicker ${WTP_SRC_FILES} )
