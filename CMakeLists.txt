cmake_minimum_required(VERSION 3.21)

# Payload Interpreter by D7ry (MIT), rebuilt on Eddoursul's CommonLibSSE-GG so it loads on
# Skyrim 1.7 (the 1.7 Address Library format). Same code, same plugin name.
project(PayloadInterpreter VERSION 1.1.1 LANGUAGES C CXX)
set(PROJECT_FRIENDLY_NAME "Payload Interpreter")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_compile_definitions(NOMINMAX _USE_MATH_DEFINES WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
add_compile_options(/permissive- /Zc:preprocessor /EHsc /utf-8)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Libraries download on the first build. To reuse copies another project already downloaded
# (FetchContent's _deps folder), configure with -DLOCAL_DEPS=<that folder>.
set(LOCAL_DEPS "" CACHE PATH "Optional folder of already downloaded <name>-src libraries")
foreach(dep directxmath directxtk rapidcsv spdlog xbyak commonlibsse)
    string(TOUPPER ${dep} DEP)
    if(LOCAL_DEPS AND EXISTS "${LOCAL_DEPS}/${dep}-src")
        set(FETCHCONTENT_SOURCE_DIR_${DEP} "${LOCAL_DEPS}/${dep}-src")
    endif()
endforeach()
include(FetchContent)
FetchContent_Declare(DirectXMath URL "https://github.com/microsoft/DirectXMath/archive/refs/tags/apr2025.tar.gz"
    OVERRIDE_FIND_PACKAGE DOWNLOAD_EXTRACT_TIMESTAMP ON EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(DirectXMath)
add_library("Microsoft::DirectXMath" ALIAS "DirectXMath")
FetchContent_Declare(DirectXTK URL "https://github.com/microsoft/DirectXTK/archive/refs/tags/jul2025.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP ON OVERRIDE_FIND_PACKAGE EXCLUDE_FROM_ALL SYSTEM
    PATCH_COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/patch_directxtk.cmake")
FetchContent_MakeAvailable(DirectXTK)
add_library("Microsoft::DirectXTK" ALIAS "DirectXTK")
FetchContent_Declare(rapidcsv URL "https://github.com/d99kris/rapidcsv/archive/refs/tags/v8.90.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP ON OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(rapidcsv)
set(RAPIDCSV_INCLUDE_DIRS ${rapidcsv_SOURCE_DIR}/src)
set(SPDLOG_INSTALL ON CACHE BOOL " " FORCE)
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL " " FORCE)
FetchContent_Declare(spdlog URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz"
    DOWNLOAD_EXTRACT_TIMESTAMP ON OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(spdlog)
FetchContent_Declare(xbyak URL "https://github.com/herumi/xbyak/archive/v7.30.tar.gz" DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_MakeAvailable(xbyak)
FetchContent_Declare(simpleini URL "https://github.com/brofield/simpleini/archive/refs/tags/v4.22.tar.gz" DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_Populate(simpleini)

set(SKSE_SUPPORT_XBYAK ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_SE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_VR OFF CACHE BOOL " " FORCE)
set(BUILD_TESTS OFF CACHE BOOL " " FORCE)
FetchContent_Declare(CommonLibSSE GIT_REPOSITORY https://github.com/eddoursul/CommonLibSSE-GG
    GIT_TAG 3aa704ee2de9fdba3a2e77cc08e3d3ed8ea6e00f)
FetchContent_MakeAvailable(CommonLibSSE)
get_target_property(COMMONLIB_SRC_DIR CommonLibSSE SOURCE_DIR)
include(${COMMONLIB_SRC_DIR}/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(${PROJECT_NAME} AUTHOR "D7ry" SOURCES ${headers} ${sources})
target_link_libraries(CommonLibSSE PUBLIC DirectXTK xbyak)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSE::CommonLibSSE spdlog xbyak)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${simpleini_SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
