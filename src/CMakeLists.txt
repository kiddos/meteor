set(ALLEGRO_PREFIX ${CMAKE_BINARY_DIR}/allegro-staging)

ExternalProject_Add(
    allegro5-external
    GIT_REPOSITORY     https://github.com/liballeg/allegro5.git
    GIT_TAG            5.2.11.3
    CMAKE_ARGS         -DCMAKE_INSTALL_PREFIX:PATH=${ALLEGRO_PREFIX}
                       -DWANT_DOCS=OFF
                       -DWANT_EXAMPLES=OFF
)

add_executable(meteor
  main.c
  game.c
  common.c
  ship.c
  bullet.c
  meteor.c
  message.c
  statusbar.c
  timedisplay.c
  score.c
  menu.c
  help.c
  record.c inputdialog.c powerup.c fx.c
)

add_dependencies(meteor allegro5-external)

target_include_directories(meteor PRIVATE 
    ${ALLEGRO_PREFIX}/include
)

target_link_directories(meteor PRIVATE ${ALLEGRO_PREFIX}/lib)

target_link_libraries(meteor PRIVATE
    m
    allegro
    allegro_primitives
    allegro_dialog
    allegro_image
    allegro_audio
    allegro_acodec
    allegro_ttf
    allegro_font
)
