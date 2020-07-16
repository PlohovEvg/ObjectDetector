cmake_minimum_required(VERSION 3.4.3)

project(ObjectDetector CXX)

find_package(InferenceEngine REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(3rdparty/gtest)

add_definitions(-DDATA_FOLDER="${PROJECT_SOURCE_DIR}/data")

file(GLOB CPPS "*.cpp")
file(GLOB DET "detector/*")

add_executable(${PROJECT_NAME} ${CPPS} ${DET})
