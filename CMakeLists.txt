cmake_minimum_required(VERSION 3.20)   # 3.20 is needed for `ctest --test-dir`
project(HospitalManagementSystem VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /utf-8)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Header-only shared code (Types.h, Status.h) + the include/ root used by every module.
add_library(hms_common INTERFACE)
target_include_directories(hms_common INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

include(CTest)               # defines BUILD_TESTING and enables ctest
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
