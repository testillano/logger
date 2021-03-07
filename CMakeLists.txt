############
# Versions #
############
cmake_minimum_required (VERSION 3.5.1)
project(ertlogger VERSION 1.0.0 LANGUAGES CXX)

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
set(ERT_LOGGER_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/ert")

##############
# Build type #
##############
include(build_type)
set_cmake_build_type()
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

# Build output directory:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY is ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY is ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY is ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

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
if (ERT_LOGGER_BuildExamples)
  add_subdirectory( examples )
endif()

###########
# Install #
###########
#SET(MY_OWN_INSTALL_PREFIX "/opt/ert-logger" CACHE PATH "Prefix prepended to install directories")
#SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
# Headers:
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ert/tracing DESTINATION include/${ERT_LOGGER_TARGET_NAME})
# Libraries:
file(GLOB_RECURSE LIBRARIES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/*)
install(FILES ${LIBRARIES} DESTINATION lib)

