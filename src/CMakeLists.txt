set(LIBRARY_NAME containerofunique)

set(SOURCE_FILES dequeofunique.h)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
