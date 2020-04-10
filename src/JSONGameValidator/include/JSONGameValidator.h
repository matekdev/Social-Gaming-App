#include<string>
#include<map>
#include <nlohmann/json.hpp>

class JSONGAMEValidator
{
  public:
    enum GameSpecification{CONFIGURATION,
                       CONSTANTS,
                       VARIABLES,
                       PER_PLAYER,
                       PER_AUDIENCE,
                       RULES
    };
    enum GameConfiguration {NAME,
                      PLAYER_COUNT,
                      AUDIENCE,
                      SETUP,
                      MIN_PLAYER,
                      MAX_PLAYER
    };

    enum StatusCode {
      VALID,
      INVALID
    };

    std::map<std::string, GameSpecification> jsonGameSpecification = 
    {
      {"configuration", GameSpecification::CONFIGURATION},
      {"constants", GameSpecification::CONSTANTS},
      {"variables", GameSpecification::VARIABLES},
      {"per-player", GameSpecification::PER_PLAYER},
      {"per-audience", GameSpecification::PER_AUDIENCE},
      {"rules", GameSpecification::RULES}
    };

    std::map<std::string, GameConfiguration> jsonGameConfiguration = 
    {
      {"name", GameConfiguration::NAME},
      {"player count", GameConfiguration::PLAYER_COUNT},
      {"audience", GameConfiguration::AUDIENCE},
      {"setup", GameConfiguration::SETUP},
      {"max", GameConfiguration::MAX_PLAYER},
      {"min", GameConfiguration::MIN_PLAYER}
    };
    StatusCode validGameJson(std::string& );
    void validateConfiguration(std::string&);
};