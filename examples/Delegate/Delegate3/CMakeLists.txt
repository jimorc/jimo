cmake_minimum_required(VERSION 3.22)

add_executable(Delegate3 
    Delegate3.cpp)

target_link_libraries(Delegate3
  PRIVATE
  jimo)

target_compile_features(Delegate3 INTERFACE cxx_std_20)