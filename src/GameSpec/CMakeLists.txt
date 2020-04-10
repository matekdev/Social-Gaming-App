add_library(gameSpec
			GameSpec.cpp
			BaseRule.cpp
			Add.cpp
			Deal.cpp
			Discard.cpp
			Extend.cpp
			ForEach.cpp
			GlobalMessage.cpp
			Inparallel.cpp
			InputChoice.cpp
			InputText.cpp
			InputVote.cpp
			Loop.cpp
			Message.cpp
			MessageParser.cpp
			StateValueParser.cpp
			Parallelfor.cpp
			Reverse.cpp
			Shuffle.cpp
			Scores.cpp
			Sort.cpp
			Switch.cpp
			Timer.cpp
			When.cpp
			StringEvaluation.cpp
)

set_target_properties(gameSpec
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

target_include_directories(gameSpec 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gameSpec
	gameState
	nlohmann_json::nlohmann_json

)
