cmake_minimum_required(VERSION 3.22)

add_executable(Event2 
  Event2.cpp)

target_link_libraries(Event2
  PRIVATE
  jimo)

target_compile_features(Event2 INTERFACE cxx_std_20)