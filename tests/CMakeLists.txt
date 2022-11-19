include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

target_compile_features(GTest::GTest INTERFACE cxx_std_20)

add_executable(jimoTest jimoTest.cpp)

target_link_libraries(jimoTest
 PRIVATE
  GTest::GTest
  jimo)

  target_compile_features(jimoTest PRIVATE cxx_std_20)
  
  add_test(jimoTests jimoTest)