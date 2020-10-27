project(hx711)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#src
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
FILE(GLOB SRC_SRC "${SRC_PATH}/*.c" ) 
include_directories(${SRC_PATH})

#wiringPi
find_library(wiringPi_LIB wiringPi)

add_executable( hx711 ${SRC_SRC} )

target_link_libraries(hx711 ${wiringPi_LIB} crypt rt pthread m)
