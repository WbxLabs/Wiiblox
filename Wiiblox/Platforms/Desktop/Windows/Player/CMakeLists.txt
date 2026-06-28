cmake_minimum_required(VERSION 3.20)
project(WiibloxPlayer)

# the executable. this is what users actually run.

set(ORIG_RCC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../roblox-master-2016/RCCService")

file(GLOB PLAYER_SOURCES ${ORIG_RCC}/*.cpp ${ORIG_RCC}/gSOAP/*.cpp)
file(GLOB PLAYER_HEADERS ${ORIG_RCC}/*.h)

add_executable(WiibloxPlayer ${PLAYER_SOURCES} ${PLAYER_HEADERS})

target_include_directories(WiibloxPlayer
    PUBLIC
        ${ORIG_RCC}
        ${ORIG_RCC}/gSOAP
)

target_link_libraries(WiibloxPlayer
    PUBLIC
        WiibloxApp
    PRIVATE
        ws2_32
        winmm
        crypt32
        imm32
        wldap32
        version
)

target_link_libraries(WiibloxPlayer
    PUBLIC
        zlibstatic
)

target_compile_definitions(WiibloxPlayer
    PUBLIC
        WIN32
        _CONSOLE
        _RELEASE
        _MBCS
        _SECURE_SCL=0
        NDEBUG
)
