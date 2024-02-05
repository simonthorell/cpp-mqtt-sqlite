#============= Find All Source Files ===============#
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/aes256_decryptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mqtt_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/salt_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite_database.cpp
)
message(STATUS "Source files: ${SOURCE_FILES}")

#================ Include Libraries ================#
add_library(${PROJECT_NAME}-lib ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}-lib PUBLIC
    ../include
    ${json_SOURCE_DIR}/include
)

#============ Link external libraries ==============#
target_link_libraries(${PROJECT_NAME}-lib PRIVATE
    nlohmann_json::nlohmann_json
    OpenSSL::Crypto
    ${PAHO_MQTT3AS_LIBRARY}
    # paho-mqtt3as
    paho-mqttpp3
    sqlite3
)

#=============== Add the executable ================#
# Only compile main.cpp for the executable
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

#========== Link against static library ============#
# Link the other libraries against the executable
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)