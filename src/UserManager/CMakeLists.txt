add_library(userManager 
    UserManager.cpp
)

target_link_libraries(userManager
    user
)

target_include_directories(userManager
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(userManager
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)