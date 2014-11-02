project(pugi)
set(SOURCES "pugixml.cpp")
set(HEADERS "pugixml.hpp" "pugiconfig.hpp")
add_library(pugi ${SOURCES} ${HEADERS})
