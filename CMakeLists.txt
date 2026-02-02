# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)
# path/to/Qt6Config.cmake or qt6-config.cmake
set(CMAKE_PREFIX_PATH /home/celina/Qt/6.10.1/gcc_64/lib/cmake/Qt6)

# std=c++23
set(CMAKE_CXX_STANDARD 23)

# Enable/Disable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# clangd tidy
find_program(CLANG_TIDY_EXE "clang-tidy")
if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
      --warnings-as-errors=-*
    )
else()
    message(WARNING "clang-tidy not found!")
endif()


# Project name and details
project(cutie VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(cutie
    src/main.cxx
    src/CutieSslServer.h
    src/CutieSslServer.cxx
)

target_include_directories(cutie
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/network
)

target_link_libraries(cutie PRIVATE Qt6::Network Qt6::Core Qt6::Widgets)
