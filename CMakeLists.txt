cmake_minimum_required(VERSION 3.10)

project(PFD)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SOURCE_FILES
    PFD.cpp
)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include  )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

