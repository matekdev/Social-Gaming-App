#include "JSONGameValidator.h"
#include <sstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <iostream>

JSONGAMEValidator::StatusCode JSONGAMEValidator::validGameJson(std::string& jsonString){
  nlohmann::json jsonObject = nlohmann::json::parse(jsonString);
  std::map<std::string, JSONGAMEValidator::GameSpecification>::iterator command;
  size_t jsonGameSpecificationMapSize = jsonGameSpecification.size();
    
  if(jsonGameSpecificationMapSize!=jsonObject.size())
  {
    return StatusCode::INVALID;
  }
  auto key=  std::find_if(jsonObject.items().begin(), jsonObject.items().end(), [](auto& elem){return jsonGameSpecification.find(elem.key()) == jsonGameSpecification.end();});
  if (key == jsonObject.items().begin()){
      return StatusCode::INVALID;
  }
  return StatusCode::VALID;
}

void JSONGAMEValidator::validateConfiguration(std::string& jsonObject){
    nlohmann::json  json  = nlohmann::json::parse(jsonObject);
    std::map<std::string, JSONGAMEValidator::GameConfiguration>::iterator command;
    command = jsonGameConfiguration.find(json.object());

    if(command == jsonGameConfiguration.end()){
     throw std::invalid_argument("Json Game specifcation invalid");
    }
}

