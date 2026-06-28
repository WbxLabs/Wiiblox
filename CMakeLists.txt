cmake_minimum_required(VERSION 3.20)
project(Wiiblox VERSION 1.0.0 LANGUAGES CXX C)

# all your base are belong to us

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -fno-rtti -Wno-unused-parameter -Wno-deprecated-declarations -Wno-register")
elseif(MSVC)
    add_compile_options(/W3 /WX- /EHsc /GS- /arch:SSE2 /fp:fast /Zc:wchar_t /Zc:forScope /MP)
endif()

add_subdirectory(Wiiblox)
