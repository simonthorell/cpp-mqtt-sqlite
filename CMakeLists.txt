#================== CMake Configuration ===================#
cmake_minimum_required(VERSION 3.28.2)
project(cpp-mqtt-sqlite) # Project name

#=================== Set C++ Standard ====================#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#================= Set Build Configuration ================#
# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)
    add_compile_options(
        -g           # Debug information
        -Werror      # Treat warnings as errors
        -Wall        # Enable all warnings
        -Wextra      # Enable extra warnings
        -pedantic    # Ensure standards-compliant code
    )

# Release configuration
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release build")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)
    add_compile_options(
        -O2          # Optimization level 2
    )
    
# Other configurations (if needed)
else()
    message("Other build configuration")
endif()

#================= Find Installed Packages ================#
find_library(PAHO_MQTT3AS_LIBRARY # Find Paho MQTT C library
    NAMES paho-mqtt3as
    PATHS /usr/local/lib
)
find_package(PahoMqttCpp REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

#==================== Fetch Libraries =====================#
# Fetch external libraries
include(FetchContent)

# Fetch the JSON library as it's a header-only library
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

#=================== Add Subdirectories ===================#
add_subdirectory(src)
add_subdirectory(tests)