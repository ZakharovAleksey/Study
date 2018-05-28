cmake_minimum_required(VERSION 3.5)

set(
	source_list
	"week_1/header_week_1.h"
	"week_1/source_week_1.cpp"
	"week_2/header_week_2.h"
	"week_2/source_week_2.cpp"
	"week_3/header_week_3.h"
	"week_3/source_week_3.cpp"
	"week_4/header_week_4.h"
	"week_4/source_week_4.cpp"
	"week_5/header_week_5.h"
	"week_5/source_week_5.cpp"
	"week_6/header_week_6.h"
	"week_6/source_week_6.cpp"
	"main.cpp"
)

add_executable(${PROJECT_NAME} ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
