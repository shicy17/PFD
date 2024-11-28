cmake_minimum_required(VERSION 3.10)

project(PFD)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include  )

add_executable(PFD PFD.cpp)
target_link_libraries(PFD ${OpenCV_LIBS})

add_executable(PFDs PFDs.cpp)
target_link_libraries(PFDs ${OpenCV_LIBS})