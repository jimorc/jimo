cmake_minimum_required(VERSION 3.22)

add_executable(ActionHandler2 
ActionHandler2.cpp)

target_link_libraries(ActionHandler2
  PRIVATE
  jimo)

target_compile_features(ActionHandler2 INTERFACE cxx_std_20)