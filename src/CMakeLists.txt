set(BUILD_TARGET meteor)


set(CMAKE_C_FLAGS "-g -Wall -O3 -DDEBUG")


set(LIBRARIES -lm -lallegro -lallegro_primitives
    -lallegro_dialog -lallegro_image -lallegro_audio
    -lallegro_acodec -lallegro_ttf -lallegro_font)

add_executable(${BUILD_TARGET} main.c game.c common.c ship.c
               bullet.c meteor.c message.c)

target_link_libraries(${BUILD_TARGET} ${LIBRARIES})

add_custom_command(TARGET ${BUILD_TARGET} POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy
                  ${PROJECT_SOURCE_DIR}/build/src/meteor
                  ${PROJECT_BINARY_DIR}/)
