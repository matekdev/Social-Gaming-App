add_library(game
        Game.cpp
        )

set_target_properties(game
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        PREFIX ""
        )

target_include_directories(
        game
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries (game
        gameSpec
        gameState
)
