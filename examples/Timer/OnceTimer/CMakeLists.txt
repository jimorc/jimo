cmake_minimum_required(VERSION 3.22)

add_executable(OnceTimer
    OnceTimer.cpp)

target_link_libraries(OnceTimer
    PRIVATE
    jimo)

target_compile_features(OnceTimer INTERFACE cxx_std_20)

if (WIN32)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-W -Wall -Werror)
endif ()
