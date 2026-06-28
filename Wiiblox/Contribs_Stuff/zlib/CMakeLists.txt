# build zlib from source because we are real programmers

set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib-1.3.2)

file(GLOB ZLIB_SOURCES ${ZLIB_DIR}/*.c)
file(GLOB ZLIB_HEADERS ${ZLIB_DIR}/*.h)

add_library(zlibstatic STATIC ${ZLIB_SOURCES} ${ZLIB_HEADERS})

target_include_directories(zlibstatic PUBLIC ${ZLIB_DIR})

# dont build the test crap
set_source_files_properties(
    ${ZLIB_DIR}/example.c
    ${ZLIB_DIR}/minigzip.c
    PROPERTIES HEADER_FILE_ONLY TRUE
)
