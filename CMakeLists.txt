cmake_minimum_required (VERSION 3.21.2)
project(comfyengine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

# I stole this from someone lol
if (${CMAKE_BUILD_TYPE} STREQUAL "WindowsFromLinux")
# cross compilers to use for C, C++ and Fortran
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

# target environment on the build host system
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# modify default behavior of FIND_XXX() commands
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

file(GLOB comfyengine_SRC
     "engine/**.cpp"
     "ui/**.cpp"
     "core/**.cpp"
)

if(NOT MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    if(${CMAKE_BUILD_TYPE} STREQUAL "WindowsFromLinux")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    endif()
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
# Enable sanitizers
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
    endif()
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()

add_executable(comfyengine ${comfyengine_SRC})
target_include_directories(comfyengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (${CMAKE_BUILD_TYPE} STREQUAL "WindowsFromLinux")
     target_link_libraries(comfyengine
        ${CMAKE_SOURCE_DIR}/ext/libraylib_windows.a
        winmm)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(comfyengine
        ${CMAKE_SOURCE_DIR}/ext/libraylib_linux.a
        dl pthread)
endif()

