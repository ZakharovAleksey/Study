# Author: Zakharov Aleksey
cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME yandex_cpp)
set(CMAKE_CXX_STANDARD 17)

project( ${PROJECT_NAME} )

add_subdirectory(time_logger) 	# Static library
add_subdirectory(unit_test) 	# Static library

set( _src_root_path ${CMAKE_CURRENT_SOURCE_DIR}/src )
add_subdirectory(src) 			# Our Excecutable


