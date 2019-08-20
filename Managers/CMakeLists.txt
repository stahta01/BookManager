cmake_minimum_required(VERSION 3.0)

set(LIB_NAME managers)
project(${LIB_NAME} CXX)

add_subdirectory(ut)


set(SRCS
	src/GeneralManager.cpp
	src/SettingsManager.cpp
    )

set(HEADERS
	include/Manager/GeneralManager.hpp
	include/Manager/SettingsManager.hpp
    )

add_library(${LIB_NAME} ${SRCS})
target_link_libraries(${LIB_NAME}
    PUBLIC
        book::factory
        maillon_creation
        lib
    )
target_include_directories(managers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        src
        ut
    )