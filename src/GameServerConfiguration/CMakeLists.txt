add_library(gameServerConfiguration
  GameServerConfiguration.cpp
)

set_target_properties(gameServerConfiguration
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(gameServerConfiguration
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries (gameServerConfiguration
  nlohmann_json::nlohmann_json
  stdc++fs

)