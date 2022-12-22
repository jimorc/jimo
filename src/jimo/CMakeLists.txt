set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(jimo
    SHARED
    src/StopWatch.cpp)

target_include_directories(jimo
    PUBLIC
    include)
    