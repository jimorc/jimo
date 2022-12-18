cmake_minimum_required(VERSION 3.22)

add_executable(Delegate 
    Delegate.cpp)

target_link_libraries(Delegate
  PRIVATE
  jimo)

target_compile_features(Delegate INTERFACE cxx_std_20)