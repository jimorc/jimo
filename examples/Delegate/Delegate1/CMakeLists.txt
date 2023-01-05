cmake_minimum_required(VERSION 3.22)

add_executable(Delegate1 
    Delegate1.cpp)

target_link_libraries(Delegate1
  PRIVATE
  jimo)

target_compile_features(Delegate1 INTERFACE cxx_std_20)