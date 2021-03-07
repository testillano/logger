############
# Versions #
############
cmake_minimum_required (VERSION 3.5.1)
project(ert_logger VERSION 1.0.5 LANGUAGES CXX)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MAIN_PROJECT ON)
endif()

#############
# Variables #
#############
option(ERT_LOGGER_BuildExamples "Build the examples." ${MAIN_PROJECT})
set(ERT_LOGGER_TARGET_NAME       ${PROJECT_NAME})
set(ERT_LOGGER_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###########
# Modules #
###########
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

# Operative system
include(os_type)
set_cmake_os_type()

# Build type
include(build_type)
set_cmake_build_type()

# Build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY is ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY is ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY is ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

# Compilation flags
include(compiler_flags)
set_cmake_compiler_flags()

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
#SET(MY_OWN_INSTALL_PREFIX "/opt/ert_logger" CACHE PATH "Prefix prepended to install directories")
#SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

include(GNUInstallDirs)
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
install(
          DIRECTORY ${ERT_LOGGER_INCLUDE_BUILD_DIR}/ert/tracing
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ert
)

# Library install, at src project

###########
# DOXYGEN #
###########
find_package(Doxygen)
IF(Doxygen_FOUND)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
ENDIF(Doxygen_FOUND)

