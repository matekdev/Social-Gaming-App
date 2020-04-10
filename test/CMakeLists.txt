add_subdirectory(lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Add your test file here
add_executable(runAllTests
    UserTest.cpp
    InvitationTest.cpp
    UserManagerTest.cpp
    GameTest.cpp
    GameSessionTest.cpp
    GameSessionListTest.cpp
    GameStateTest.cpp
)

# Link the appropriate library
target_link_libraries(runAllTests
        gmock
        gtest
        gtest_main
        user
        invitation
        userManager
        game
        gameSession
        gameSessionList
        gameState
)

add_custom_command(TARGET runAllTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../test/testgames
        ${PROJECT_BINARY_DIR}/testgames
)

add_test(NAME AllTests COMMAND runAllTests)