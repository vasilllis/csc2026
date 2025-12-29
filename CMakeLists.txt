cmake_minimum_required(VERSION 3.16)
project(csc2026 LANGUAGES CXX)

# ============================================================================
# Options
# ============================================================================
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelism" ON)

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build microbenchmarks" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ============================================================================
# Compiler warnings
# ============================================================================
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()

# ============================================================================
# Sanitizers
# ============================================================================
if(ENABLE_SANITIZERS AND ENABLE_TSAN)
  message(FATAL_ERROR "ENABLE_SANITIZERS and ENABLE_TSAN are mutually exclusive.")
endif()

if(ENABLE_SANITIZERS)
  message(STATUS "Sanitizers: Address + Undefined")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
  add_link_options(-fsanitize=address,undefined)
endif()

if(ENABLE_TSAN)
  message(STATUS "Sanitizers: Thread")
  add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -g)
  add_link_options(-fsanitize=thread)
endif()

# ============================================================================
# OpenMP
# ============================================================================
if(ENABLE_OPENMP)
  find_package(OpenMP)
  if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP: enabled")
    add_compile_definitions(CSC2026_USE_OPENMP=1)
  else()
    message(WARNING "OpenMP requested but not found. Building without OpenMP.")
  endif()
else()
  message(STATUS "OpenMP: disabled")
endif()

# ============================================================================
# Dependencies: Catch2 (tests) and Google Benchmark (benchmarks)
# ============================================================================
include(FetchContent)

if(BUILD_TESTS)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
  )
  FetchContent_MakeAvailable(Catch2)
  include(CTest)
endif()

if(BUILD_BENCHMARKS)
  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
  )
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(benchmark)
endif()

# ============================================================================
# Library: csc2026
# ============================================================================
add_library(csc2026
  src/TrackReconstructor.cpp
  src/EventProcessor.cpp
  src/Particle.cpp
)

target_include_directories(csc2026 PUBLIC include)

if(ENABLE_OPENMP AND OpenMP_CXX_FOUND)
  target_link_libraries(csc2026 PUBLIC OpenMP::OpenMP_CXX)
endif()

# ============================================================================
# Main executables
# ============================================================================
add_executable(analyze src/main.cpp)
target_link_libraries(analyze PRIVATE csc2026)

add_executable(event_processor src/event_processor_main.cpp)
target_link_libraries(event_processor PRIVATE csc2026)

# ============================================================================
# Tests
# ============================================================================
if(BUILD_TESTS)
  add_executable(test_track_reconstructor tests/test_track_reconstructor.cpp)
  target_link_libraries(test_track_reconstructor PRIVATE csc2026 Catch2::Catch2WithMain)
  add_test(NAME track_reconstructor COMMAND test_track_reconstructor)

  add_executable(test_event_processor tests/test_event_processor.cpp)
  target_link_libraries(test_event_processor PRIVATE csc2026 Catch2::Catch2WithMain)
  add_test(NAME event_processor COMMAND test_event_processor)

  add_executable(test_particle tests/test_particle.cpp)
  target_link_libraries(test_particle PRIVATE csc2026 Catch2::Catch2WithMain)
  add_test(NAME particle COMMAND test_particle)
endif()

# ============================================================================
# Benchmarks
# ============================================================================
if(BUILD_BENCHMARKS)
  add_executable(benchmarks
    benchmarks/bench_event_processor.cpp
    benchmarks/bench_particle.cpp
  )
  target_link_libraries(benchmarks PRIVATE csc2026 benchmark::benchmark)
endif()

