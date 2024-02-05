#============= Find All Source Files ===============#
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

#============ Find External Libraries ==============#
add_library(MyProjectLib ${SOURCE_FILES})
target_include_directories(MyProjectLib PUBLIC
    ../include PRIVATE 
    ${json_SOURCE_DIR}/include
)

#============ Link external libraries ==============#
target_link_libraries(MyProjectLib PRIVATE
    nlohmann_json::nlohmann_json
)

#=============== Add the executable ================#
add_executable(MyProject ${SOURCE_FILES})
target_include_directories(MyProject PUBLIC "../include")

#========= Link against static libraries ===========#
# Uncomment to link against external libraries
target_link_libraries(MyProject PRIVATE 
    MyProjectLib # Link against the static library
)