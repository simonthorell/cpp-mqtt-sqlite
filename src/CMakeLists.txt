#============= Find All Source Files ===============#
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_file.cpp
)

message(STATUS "Source files: ${SOURCE_FILES}")

#================= Find Libraries =================#
add_library(${PROJECT_NAME}-lib ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}-lib PUBLIC
    ../include
    ${json_SOURCE_DIR}/include
)

#============ Link external libraries ==============#
target_link_libraries(${PROJECT_NAME}-lib PRIVATE
    nlohmann_json::nlohmann_json
    OpenSSL::Crypto
    paho-mqtt3as
    paho-mqttpp3
    sqlite3
)

#=============== Add the executable ================#
add_executable(${PROJECT_NAME} main.cpp) # Only compile main.cpp for the executable
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

#========= Link against static libraries ===========#
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)