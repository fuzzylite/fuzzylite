cmake_minimum_required(VERSION 2.8)

project(fuzzylite)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(.)

file(STRINGS FL_HEADERS fl-headers)
file(STRINGS FL_SOURCES fl-sources)

string(REGEX REPLACE "\n" " " ${fl-headers} ${fl-headers})
string(REGEX REPLACE "\n" " " ${fl-sources} ${fl-sources})


add_library(fl-shared SHARED ${fl-headers} ${fl-sources})
set_target_properties(fl-shared PROPERTIES OUTPUT_NAME fuzzylite)

add_library(fl-static STATIC ${fl-headers} ${fl-sources})
set_target_properties(fl-static PROPERTIES OUTPUT_NAME fuzzylite)


add_executable(fl-bin src/main.cpp)
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite)
target_link_libraries(fl-bin fl-shared)


install(TARGETS fl-bin RUNTIME DESTINATION bin)
install(TARGETS fl-shared DESTINATION lib)
install(TARGETS fl-static DESTINATION lib)
install(FILES ${fl-headers} DESTINATION include/fl)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

