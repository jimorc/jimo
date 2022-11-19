cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(jimo
    VERSION 0.1
    DESCRIPTION "A library containing general purpose classes"
    LANGUAGES CXX)

add_compile_options(-W -Wall -Werror)

add_subdirectory(src)
add_subdirectory(tests)