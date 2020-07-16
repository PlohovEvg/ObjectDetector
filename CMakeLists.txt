cmake_minimum_required(VERSION 3.4.3)

project(ObjectDetector CXX)

find_package(InferenceEngine REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(3rdparty/gtest)

file(GLOB MAIN "main.cpp")

add_definitions(-DDATA_FOLDER="${PROJECT_SOURCE_DIR}/data")
file(GLOB SOURCES "detector/*.cpp")
if (SOURCES)
	add_library(module_${module_name} STATIC ${SOURCES} ${MAIN})
	target_include_directories(module_${module_name} PUBLIC "${module}/include")
	target_link_libraries(module_${module_name} PUBLIC ${OpenCV_LIBRARIES} ${InferenceEngine_LIBRARIES})
endif()


