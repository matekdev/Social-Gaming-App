add_library(jsonValidator
  JSONValidator.cpp 
)


target_include_directories(jsonValidator
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(jsonValidator
  nlohmann_json::nlohmann_json
)
