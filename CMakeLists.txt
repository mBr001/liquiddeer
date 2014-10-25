cmake_minimum_required(VERSION 2.6)
project(liquiddeer)

include_directories("pugi/" "gumbo/")

set(SRC_LIST source/deer.cpp
             source/auth.cpp
             source/config.cpp
             source/interact.cpp
             source/parse.cpp
             source/sync.cpp
             source/xml.cpp
             source/track.cpp
             source/pugi/pugixml.cpp
             source/gumbo/attribute.c
             source/gumbo/char_ref.c
             source/gumbo/error.c
             source/gumbo/parser.c
             source/gumbo/string_buffer.c
             source/gumbo/string_piece.c
             source/gumbo/tag.c
             source/gumbo/tokenizer.c
             source/gumbo/utf8.c
             source/gumbo/util.c
             source/gumbo/vector.c
             )
set(CMAKE_C_FLAGS "-std=c99 -Wall") 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/source/cmake/Modules/)

FIND_PACKAGE(CURL REQUIRED)
FIND_PACKAGE(Jansson REQUIRED)

include_directories(${CURL_INCLUDE_DIRS} ${JANSSON_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${JANSSON_LIBRARIES} ${CURL_LIBRARIES})
