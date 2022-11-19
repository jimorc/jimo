add_library(jimo
    SHARED
    src/StopWatch.cpp
    include/StopWatch.h)

target_include_directories(jimo
    PUBLIC
    include)

target_compile_features(jimo PRIVATE cxx_std_20)