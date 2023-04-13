cmake_minimum_required(VERSION 3.10)

project(Faraday VERSION 1.0)

configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in ${PROJECT_SOURCE_DIR}/include/config.h)

include_directories(include)
file(GLOB SOURCES "src/*.c")

add_executable(Faraday ${SOURCES} "src/faraday/main.c")
target_include_directories(Faraday PUBLIC "${PROJECT_BINARY_DIR}/include")

add_executable(Lister ${SOURCES} "src/lister/lister.c")
target_include_directories(Lister PUBLIC "${PROJECT_BINARY_DIR}/include")

add_executable(Compressor ${SOURCES} "src/compressor/compressor.c")
target_include_directories(Compressor PUBLIC "${PROJECT_BINARY_DIR}/include")

add_executable(CrossReferencer ${SOURCES} "src/cross_referencer/cross_referencer.c")
target_include_directories(CrossReferencer PUBLIC "${PROJECT_BINARY_DIR}/include")

add_executable(Cruncher ${SOURCES} "src/cruncher/cruncher.c")
target_include_directories(Cruncher PUBLIC "${PROJECT_BINARY_DIR}/include")


install(TARGETS Faraday Lister Compressor CrossReferencer Cruncher DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/include/config.h"
        DESTINATION include)

include(CTest)
