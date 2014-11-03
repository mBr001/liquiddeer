cmake_minimum_required(VERSION 2.6)
project(liquiddeer)

add_subdirectory(source/pugi)
add_subdirectory(source/gumbo)

set(SRC_LIST "source/deer.c"
#             source/auth.cpp
             "source/config.c"
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
find_package(LibXml2 REQUIRED)
if(NOT LIBXML2_FOUND)
    message(SEND_ERROR "Failed to find LibXml2")
    return()
else()
    include_directories(${LIBXML2_INCLUDE_DIR})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib-debug)
  set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/lib-debug)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin-dbg)
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
  set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/lib)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()

include_directories(${CURL_INCLUDE_DIRS} ${JANSSON_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${JANSSON_LIBRARIES} ${CURL_LIBRARIES} ${GLIB_LIBRARIES} ${LIBXML2_LIBRARIES} pugi gumbo)
