add_library(gameServer GameServer.cpp)

set_target_properties(gameServer
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(gameServer
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)

target_link_libraries(gameServer
  ${CMAKE_THREAD_LIBS_INIT}

  networking
  gameSessionList
  gameServerConfiguration
)
