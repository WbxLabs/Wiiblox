cmake_minimum_required(VERSION 3.20)
project(WiibloxCore)

set(ORIG_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Base")
set(ORIG_LOG "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../roblox-master-2016/Log")

file(GLOB LOG_SOURCES ${ORIG_LOG}/*.cpp)

add_library(WiibloxCore STATIC
    ${ORIG_BASE}/cpucount.cpp
    ${ORIG_BASE}/RbxAssert.cpp
    ${ORIG_BASE}/RbxFormat.cpp
    ${ORIG_BASE}/rbx/MathUtil.cpp
    ${ORIG_BASE}/rbx/Time.cpp
    ${ORIG_BASE}/rbx/boost.cpp
    ${ORIG_BASE}/rbx/CEvent.cpp
    ${ORIG_BASE}/rbx/Crypt.cpp
    ${ORIG_BASE}/rbx/Memory.cpp
    ${ORIG_BASE}/rbx/ProcessPerfCounter.cpp
    ${ORIG_BASE}/rbx/Profiler.cpp
    ${ORIG_BASE}/rbx/Signal.cpp
    ${ORIG_BASE}/rbx/TaskScheduler.cpp
    ${ORIG_BASE}/rbx/TaskScheduler.Job.cpp
    ${ORIG_BASE}/rbx/TaskScheduler.Thread.cpp
    ${ORIG_BASE}/rbx/ThreadSafe.cpp
    ${ORIG_BASE}/rbx/Tasks/Coordinator.cpp
    ${ORIG_BASE}/util/Debug.cpp
    ${ORIG_BASE}/util/Log.cpp
    ${ORIG_BASE}/util/RbxCrash.cpp
    ${ORIG_BASE}/util/RegistryUtil.cpp
    ${ORIG_BASE}/include/rbx/RbxDbgInfo.cpp
    ${LOG_SOURCES}
)

target_include_directories(WiibloxCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../Base/include
        ${ORIG_LOG}/include
    PRIVATE
        ${ORIG_BASE}
        ${ORIG_BASE}/include
)

target_compile_definitions(WiibloxCore PUBLIC WIN32 _LIB _RELEASE _MBCS _SECURE_SCL=0)
target_compile_definitions(WiibloxCore PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

target_link_libraries(WiibloxCore PUBLIC WiibloxBoost)
