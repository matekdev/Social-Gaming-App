add_library(gameSessionList
  GameSessionList.cpp
)

set_target_properties(gameSessionList
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)


target_include_directories(gameSessionList
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries (gameSessionList
    invitation
    gameSession
    userManager
)