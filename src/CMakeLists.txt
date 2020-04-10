add_subdirectory(Invitation)
add_subdirectory(User)
add_subdirectory(Game)
add_subdirectory(GameGeneralConfig)
add_subdirectory(GameState)
add_subdirectory(GameSpec)
add_subdirectory(UserManager)
add_subdirectory(GameSession)
add_subdirectory(GameSessionList)
add_subdirectory(GameServerConfiguration)
add_subdirectory(GameServer)
add_subdirectory(GameParser)

################################

add_executable(GameServer
  main.cpp
)

set_target_properties(GameServer
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(GameServer
  ${CMAKE_THREAD_LIBS_INIT}
  gameServer
  gameServerConfiguration
)

add_custom_command(TARGET GameServer POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/ServerProperties.json
                       ${PROJECT_BINARY_DIR}/config/ServerProperties.json
                   )

add_custom_command(TARGET GameServer POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/socialapp.html
                       ${PROJECT_BINARY_DIR}/socialapp.html
                   )

add_custom_command(TARGET GameServer POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/Games
                       ${PROJECT_BINARY_DIR}/Games
                   )

install(FILES ServerProperties.json DESTINATION config)

install(TARGETS GameServer
  RUNTIME DESTINATION bin
)