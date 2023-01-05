cmake_minimum_required(VERSION 3.22)

add_executable(Delegate2 
    Delegate2.cpp)

target_link_libraries(Delegate2
  PRIVATE
  jimo)

target_compile_features(Delegate2 INTERFACE cxx_std_20)