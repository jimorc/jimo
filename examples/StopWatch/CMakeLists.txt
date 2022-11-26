cmake_minimum_required(VERSION 3.22)

add_executable(StopWatch 
  StopWatch.cpp)

target_link_libraries(StopWatch
  PRIVATE
  jimo)

target_compile_features(StopWatch INTERFACE cxx_std_20)