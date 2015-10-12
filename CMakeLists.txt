cmake_minimum_required(VERSION 2.8)
project( SpreadMaximization )


if(NOT ${CMAKE_VERSION} VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set(SRCS
#     src/main.cpp
    src/test.cpp
#     src/DisplayImage.cpp
)


#find_package(OpenCV 3.0 REQUIRED COMPONENTS calib3d core features2d highgui imgproc photo)
find_package( OpenCV REQUIRED )
# set(OpenCV_DIR /usr/local/lib)


include_directories(src)
add_executable( SpreadMaximization ${SRCS} )
target_link_libraries( SpreadMaximization ${OpenCV_LIBS} )
add_subdirectory(src/utils)