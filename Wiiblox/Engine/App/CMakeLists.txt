cmake_minimum_required(VERSION 3.20)
project(WiibloxApp)

set(ORIG_APP "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/App")
set(ORIG_CSG "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/CSG")
set(ORIG_BP "${CMAKE_CURRENT_SOURCE_DIR}/../../App/BulletPhysics")
set(ORIG_REND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Rendering")
set(ORIG_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Base")
set(ORIG_LOG "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Log")
set(ORIG_NET "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Network")

# App source files
file(GLOB_RECURSE LUA_C_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Essentials/Lua/src/*.c)
set_source_files_properties(${LUA_C_SOURCES} PROPERTIES LANGUAGE CXX)

file(GLOB_RECURSE APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c
    ${LUA_C_SOURCES}
)

# Network source files
file(GLOB_RECURSE NET_SOURCES ${ORIG_NET}/*.cpp)

# CSG source files (excluding CSGKernel.cpp which has deep App/Rendering deps)
file(GLOB_RECURSE CSG_SOURCES ${ORIG_CSG}/*.cpp ${ORIG_CSG}/*.c)
list(FILTER CSG_SOURCES EXCLUDE REGEX "CSGKernel\\.cpp$")

# BulletPhysics
file(GLOB_RECURSE BP_SOURCES ${ORIG_BP}/*.cpp)

# GFX / Rendering source files
file(GLOB_RECURSE GFX_SOURCES
    ${ORIG_REND}/GfxBase/*.cpp ${ORIG_REND}/GfxBase/*.c
    ${ORIG_REND}/GfxCore/*.cpp ${ORIG_REND}/GfxCore/*.c
    ${ORIG_REND}/GfxRender/*.cpp ${ORIG_REND}/GfxRender/*.c
    ${ORIG_REND}/AppDraw/*.cpp ${ORIG_REND}/AppDraw/*.c
    ${ORIG_REND}/g3d/*.cpp ${ORIG_REND}/g3d/*.c
    ${ORIG_REND}/RbxG3D/*.cpp ${ORIG_REND}/RbxG3D/*.c
)

add_library(WiibloxApp STATIC ${APP_SOURCES} ${NET_SOURCES} ${CSG_SOURCES} ${BP_SOURCES} ${GFX_SOURCES})

target_include_directories(WiibloxApp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/Essentials/Lua/src
        ${ORIG_NET}
        ${ORIG_NET}/include
        ${ORIG_NET}/raknet/Source
        ${ORIG_CSG}
        ${ORIG_CSG}/../App
        ${ORIG_CSG}/../App/include
        ${ORIG_BP}
        ${ORIG_BP}/BulletCollision
        ${ORIG_BP}/BulletDynamics
        ${ORIG_BP}/BulletMultiThreaded
        ${ORIG_BP}/BulletSoftBody
        ${ORIG_BP}/LinearMath
        ${ORIG_REND}
        ${ORIG_REND}/GfxBase
        ${ORIG_REND}/GfxBase/include
        ${ORIG_REND}/GfxCore
        ${ORIG_REND}/GfxCore/include
        ${ORIG_REND}/GfxRender
        ${ORIG_REND}/GfxRender/include
        ${ORIG_REND}/AppDraw
        ${ORIG_REND}/AppDraw/include
        ${ORIG_REND}/g3d/include
        ${ORIG_REND}/g3d
        ${ORIG_REND}/RbxG3D
        ${ORIG_REND}/RbxG3D/include
        ${ORIG_BASE}/include
        ${ORIG_LOG}/include
)

target_compile_definitions(WiibloxApp
    PUBLIC
        WIN32 _LIB _RELEASE _MBCS _SECURE_SCL=0
        RBX_NETWORK RBX_PLATFORM_WIN SCRIPTED_NETWORK_TEST
        RAKNET_LIBTCP
        _HAS_STD_BYTE=0
    PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)

# Force-include stdafx.h so namespace alias wbx=WBX and Base using-declarations
# are visible in every translation unit (MSVC case-insensitivity compat for Clang)
target_compile_options(WiibloxApp PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/Include/stdafx.h")

target_link_libraries(WiibloxApp
    PUBLIC
        WiibloxCore
        WiibloxBoost
        zlibstatic
    PRIVATE
        ws2_32 winmm crypt32 imm32 wldap32 version
)
