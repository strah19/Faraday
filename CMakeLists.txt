cmake_minimum_required(VERSION 3.10)

project(Faraday VERSION 1.0)

configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in ${PROJECT_SOURCE_DIR}/include/config.h)

include_directories(include)
file(GLOB SOURCES "src/*.c")

add_executable(Faraday ${SOURCES})
target_include_directories(Faraday PUBLIC "${PROJECT_BINARY_DIR}/include")

add_executable(Lister "src/lister/lister.c")
target_include_directories(Lister PUBLIC "${PROJECT_BINARY_DIR}/include")

install(TARGETS Faraday Lister DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/include/config.h"
        DESTINATION include)

include(CTest)