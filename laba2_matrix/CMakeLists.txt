cmake_minimum_required(VERSION 3.20)
project(laba2)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

add_executable(laba2
                    main.cpp "stb_image.h" "Func_image.h" "Func_image.cpp" "Layer.h" "Layer.cpp" "Network.h" "Network.cpp" "Mnist.h" "Mnist.cpp")
target_link_libraries(laba2 ${Python3_LIBRARIES} -larmadillo )