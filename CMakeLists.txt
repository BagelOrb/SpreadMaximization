cmake_minimum_required(VERSION 2.8)
project( SpreadMaximization )


if(NOT ${CMAKE_VERSION} VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set(SRCS
    src/main.cpp
    src/test.cpp
)


#find_package( OpenCV REQUIRED )
include_directories(src)
add_executable( SpreadMaximization ${SRCS} )
#target_link_libraries( SpreadMaximization ${OpenCV_LIBS} )
target_link_libraries( SpreadMaximization )