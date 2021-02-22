############
# Versions #
############
cmake_minimum_required (VERSION 3.5.1)
project(logger VERSION 1.0.0 LANGUAGES CXX)

###########
# Modules #
###########
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

##############
# Build type #
##############
include(build_type)
set_cmake_build_type()
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

################
# C++ Standard #
################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#########
# Flags #
#########
add_compile_options(
    "-Wall"
    "-Werror"
    $<$<CONFIG:Debug>:--coverage>
)

link_libraries(
    $<$<CONFIG:Debug>:--coverage>
)

##################
# Subdirectories #
##################
add_subdirectory( src )
add_subdirectory( examples )

