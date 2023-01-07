cmake_minimum_required(VERSION 3.22)

add_executable(Timer1
Timer1.cpp)

target_link_libraries(Timer1
    PRIVATE
    jimo)

target_compile_features(Timer1 INTERFACE cxx_std_20)

if (WIN32)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-W -Wall -Werror)
endif ()
