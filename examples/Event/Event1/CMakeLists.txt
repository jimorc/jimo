cmake_minimum_required(VERSION 3.22)

add_executable(Event1 
  Event1.cpp)

target_link_libraries(Event1
  PRIVATE
  jimo)

target_compile_features(Event1 INTERFACE cxx_std_20)