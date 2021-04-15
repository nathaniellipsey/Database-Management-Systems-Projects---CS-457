cmake_minimum_required(VERSION 2.8.9)

project(project3)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(my_lib_sources src/main.cpp)

add_library(my_lib ${my_lib_sources})

add_executable(project3 src/main.cpp)

target_link_libraries(project3 my_lib)

install(TARGETS project3 my_lib
	RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/devel/bin
	LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/devel/lib
	ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/devel/lib/static)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${PROJECT_SOURCE_DIR}/devel/include
	FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
