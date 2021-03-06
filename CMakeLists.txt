#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

cmake_minimum_required(VERSION 2.8)
project(xlsxwriterpp CXX)

# OPTIONS
# -------

option(BUILD_STATIC "Build static libxlsxwriter" ON)

# CONFIGURATIONS
# --------------

set(CMAKE_CXX_STANDARD 11)
if(BUILD_STATIC)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif(MINGW OR MSYS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -Wno-char-subscripts -Wno-long-long")
        add_definitions(-DUSE_FILE32API)
    elseif(MSVC)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE} /MTd /O0")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT /Ox")
    endif()
endif()

# LIBRARY
# -------

set(LXWPP_SOURCES
    src/chart.cpp
    src/format.cpp
    src/utility.cpp
    src/workbook.cpp
    src/worksheet.cpp
)

add_subdirectory(libxlsxwriter)
include_directories(include)
include_directories(libxlsxwriter/include)

if(BUILD_STATIC)
    add_library(xlsxwriterpp STATIC ${LXWPP_SOURCES})
else()
    add_library(xlsxwriterpp SHARED ${LXWPP_SOURCES})
endif()
