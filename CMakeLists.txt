cmake_minimum_required(VERSION 3.4.3)

project(ObjectDetector)

find_package(InferenceEngine REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_definitions(-DDATA_FOLDER="${PROJECT_SOURCE_DIR}/data")


    # Build library
    file(GLOB SOURCES "src/*.cpp")
    if (SOURCES)
        add_library(src_files STATIC ${SOURCES})
        target_include_directories(src_files PUBLIC "include")
        target_link_libraries(src_files PUBLIC ${OpenCV_LIBRARIES} ${InferenceEngine_LIBRARIES})
    endif()  

    file(GLOB MAIN "main/*.cpp")
    if(MAIN)
          add_executable(exec_files "${MAIN}")
          target_link_libraries(exec_files PRIVATE src_files)
    endif()