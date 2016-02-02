cmake_minimum_required(VERSION 2.8)

project(meteor)

add_custom_target(RESOURCES ALL
                  ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_SOURCE_DIR}/res
                  ${PROJECT_BINARY_DIR}/res SOURCES ${RESOURCES})

add_custom_target(CONFIG ALL
                  ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_SOURCE_DIR}/config
                  ${PROJECT_BINARY_DIR}/config SOURCES ${CONFIG})

add_subdirectory(src)
