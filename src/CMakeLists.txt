add_library (logger
  ${CMAKE_CURRENT_LIST_DIR}/Logger.cc
)

target_include_directories (logger
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
