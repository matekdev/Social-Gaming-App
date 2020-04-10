#ifndef GAMEGENERALCONFIG_H
#define GAMEGENERALCONFIG_H

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <boost/variant.hpp>

namespace Configuration {
    struct PlayerCount {
        int min;
        int max;
    };

    // TODO: Modify setup value to correspond to project requirements
    struct SetupValue {
        boost::variant<std::string, int,
                bool, double, std::vector<std::string>,
                std::unordered_map<std::string, std::string>> value;
    };
};

using Setup = std::unordered_map<std::string, int>;


class GameConfig {
    public:
        GameConfig();
        GameConfig(const std::string& );

        std::string getName() const;
        Configuration::PlayerCount getPlayerCount() const;
        bool hasAudience() const;
        Setup getSetup() const;

        void setName(const std::string& );
        void setPlayerCount(const Configuration::PlayerCount& );
        void setAudience(bool );
        void setSetup(const Setup& );

        void parseConfig(const nlohmann::json&_);
    private:
        std::string name;
        Configuration::PlayerCount playerCount;
        bool audience;
		Setup setup;
};
#endif
