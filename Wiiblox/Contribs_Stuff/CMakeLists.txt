# contribs. all of them. in one place. such a beauty!

add_subdirectory(zlib)

# curl + openssl bundled together like peanut butter and jelly
set(CURL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/curl/curl-8.21.0_1-win64-mingw)

add_library(WiibloxCURL STATIC IMPORTED GLOBAL)
set_target_properties(WiibloxCURL PROPERTIES
    IMPORTED_LOCATION ${CURL_DIR}/lib/libcurl.a
    INTERFACE_INCLUDE_DIRECTORIES ${CURL_DIR}/include
)

add_library(WiibloxOpenSSL STATIC IMPORTED GLOBAL)
set_target_properties(WiibloxOpenSSL PROPERTIES
    IMPORTED_LOCATION ${CURL_DIR}/lib/libcrypto.a
    INTERFACE_INCLUDE_DIRECTORIES ${CURL_DIR}/include
)

add_library(WiibloxSSL STATIC IMPORTED GLOBAL)
set_target_properties(WiibloxSSL PROPERTIES
    IMPORTED_LOCATION ${CURL_DIR}/lib/libssl.a
    INTERFACE_INCLUDE_DIRECTORIES ${CURL_DIR}/include
)

add_library(WiibloxZstd STATIC IMPORTED GLOBAL)
set_target_properties(WiibloxZstd PROPERTIES
    IMPORTED_LOCATION ${CURL_DIR}/lib/libzstd.a
)

# nanosvg - single header svg rasterizer
# best thing since sliced bread
add_library(nanosvg INTERFACE)
target_include_directories(nanosvg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# stb_image - reads everything including webp and gif
# all hail sean barrett
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Boost - not all platforms ship cmake configs so we make our own
set(BOOST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost_1_87_0)
add_library(WiibloxBoost INTERFACE)
target_include_directories(WiibloxBoost INTERFACE ${BOOST_DIR})
target_compile_definitions(WiibloxBoost INTERFACE BOOST_ERROR_CODE_HEADER_ONLY BOOST_NO_CXX98_FUNCTION_BASE __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS)
target_compile_definitions(WiibloxBoost INTERFACE $<$<CXX_COMPILER_ID:Clang>:_CRT_SECURE_NO_WARNINGS>)
