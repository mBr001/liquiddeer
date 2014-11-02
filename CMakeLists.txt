cmake_minimum_required(VERSION 2.6)
project(liquiddeer)

add_subdirectory(source/pugi)
add_subdirectory(source/gumbo)

set(SRC_LIST "source/deer.c"
#             source/auth.cpp
            #  "source/config.cpp"
             "source/interact.c"
            # source/parse.cpp
#             source/sync.cpp
#             source/xml.cpp
#             source/track.cpp
             )
set(HEADERS #"source/config.h"
            "source/interact.h"
            )
set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/source/cmake/Modules/)

FIND_PACKAGE(CURL REQUIRED)
FIND_PACKAGE(Jansson REQUIRED)
FIND_PACKAGE(Glib REQUIRED)


include_directories(${CURL_INCLUDE_DIRS} ${JANSSON_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${JANSSON_LIBRARIES} ${CURL_LIBRARIES} ${GLIB_LIBRARIES} pugi gumbo)
