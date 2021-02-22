add_library (${ERT_LOGGER_TARGET_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
)

target_include_directories (${ERT_LOGGER_TARGET_NAME}
  PUBLIC ${ERT_LOGGER_INCLUDE_BUILD_DIR}
)
