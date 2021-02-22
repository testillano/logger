############
# Versions #
############
cmake_minimum_required (VERSION 3.5.1)
project(ert_logger VERSION 1.0.0 LANGUAGES CXX)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MAIN_PROJECT ON)
endif()

###########
# Modules #
###########
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

###########################
# Options & Configuration #
###########################
option(ERT_LOGGER_BuildExamples "Build the examples." ${MAIN_PROJECT})
set(ERT_LOGGER_TARGET_NAME       ${PROJECT_NAME})
set(ERT_LOGGER_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")

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

############
# Includes #
############
#add_library(${ERT_LOGGER_TARGET_NAME} INTERFACE)
#add_library(${PROJECT_NAME}::${ERT_LOGGER_TARGET_NAME} ALIAS ${ERT_LOGGER_TARGET_NAME})
#target_include_directories(
#    ${ERT_LOGGER_TARGET_NAME}
#    INTERFACE
#    $<BUILD_INTERFACE:${ERT_LOGGER_INCLUDE_BUILD_DIR}>
#    $<INSTALL_INTERFACE:include>
#)

##################
# Subdirectories #
##################
add_subdirectory( src )
if (ERT_LOGGER_BuildExamples)
  add_subdirectory( examples )
endif()

