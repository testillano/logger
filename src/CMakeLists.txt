add_library (tst-logger
  ${CMAKE_CURRENT_LIST_DIR}/Logger.cc
)

target_include_directories (tst-logger
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
