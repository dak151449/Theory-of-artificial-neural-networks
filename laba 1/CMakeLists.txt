cmake_minimum_required(VERSION 3.20)
project(laba1)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

add_executable(laba1
                    main.cpp "stb_image.h" "Func_image.h" "Func_image.cpp")
target_link_libraries(laba1 ${Python3_LIBRARIES} -larmadillo )