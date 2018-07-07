cmake_minimum_required(VERSION 3.5)

set(
	source_list
	"week_1/week_1.h"
	"week_1/week_1.cpp"
	"week_2/week_2.h"
	"week_2/week_2.cpp"
	"week_3/week_3.h"
	"week_3/week_3.cpp"
	"week_4/week_4.h"
	"week_4/week_4.cpp"
	"week_5/week_5.h"
	"week_5/week_5.cpp"
	"week_6/condition_parser.h"
	"week_6/condition_parser.cpp"
	"week_6/condition_parser_test.cpp"
	"week_6/database.h"
	"week_6/database.cpp"
	"week_6/database_test.cpp"
	"week_6/date.h"
	"week_6/date.cpp"
	"week_6/node.h"
	"week_6/node.cpp"
	"week_6/test_runner.h"
	"week_6/token.h"
	"week_6/token.cpp"
	"unit_test/unit_test.h"
	"unit_test/unit_test.cpp"	
	"main.cpp"
)

add_executable(${PROJECT_NAME} ${source_list})

foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
