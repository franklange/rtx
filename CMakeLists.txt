cmake_minimum_required(VERSION 3.10)
project(rtx_top)

find_package(Threads REQUIRED)

option(RTX_TESTS "Enable Tests" OFF)
option(RTX_EXAMPLES "Enable Examples" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()

add_subdirectory(third_party/bln_log)
add_subdirectory(third_party/bln_net)

# RTX lib
set(RTX_LIB rtx)
add_library(${RTX_LIB} "")

set_target_properties(${RTX_LIB} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
)

target_sources(${RTX_LIB} PRIVATE
    include/rtx/broker.hpp
    include/rtx/client.hpp
    include/rtx/log.hpp
    include/rtx/message.hpp
    include/rtx/types.hpp

    src/broker.cpp
    src/client.cpp
    src/message.cpp
)

target_include_directories(${RTX_LIB} PUBLIC include)
target_compile_features(${RTX_LIB} PUBLIC cxx_std_20)
target_link_libraries(${RTX_LIB} PUBLIC bln_log bln_net)

# RTX daemon
set(RTX_DAEMON rtxd)
add_executable(${RTX_DAEMON} src/daemon.cpp)

set_target_properties(${RTX_DAEMON} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)

target_compile_features(${RTX_DAEMON} PUBLIC cxx_std_20)
target_link_libraries(${RTX_DAEMON} PUBLIC rtx)

# Log Level
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${RTX_LIB}    PUBLIC  BLN_LOG_DBG)
    target_compile_definitions(${RTX_DAEMON} PRIVATE BLN_LOG_DBG)
endif()

if (RTX_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (RTX_EXAMPLES)
    add_subdirectory(example)
endif()
