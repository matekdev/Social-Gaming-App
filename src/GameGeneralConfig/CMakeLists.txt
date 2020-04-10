add_library(gameConfig
        GameConfig.cpp
        )

set_target_properties(gameConfig
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        PREFIX ""
        )

target_include_directories(
        gameConfig
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
        gameConfig
        nlohmann_json::nlohmann_json
)