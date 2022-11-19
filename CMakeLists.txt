cmake_minimum_required(VERSION 3.22)

project(jimo
    VERSION 0.1
    DESCRIPTION "A library containing general purpose classes"
    LANGUAGES CXX)

add_compile_options(-W -Wall -Werror)

add_subdirectory(src)