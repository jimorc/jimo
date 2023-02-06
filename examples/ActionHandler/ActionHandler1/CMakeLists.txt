cmake_minimum_required(VERSION 3.22)

add_executable(ActionHandler1 
ActionHandler1.cpp)

target_link_libraries(ActionHandler1
  PRIVATE
  jimo)

target_compile_features(ActionHandler1 INTERFACE cxx_std_20)