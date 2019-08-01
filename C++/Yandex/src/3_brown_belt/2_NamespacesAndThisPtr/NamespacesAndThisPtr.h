#pragma once 

#include"xml.h"
#include"json.h"

namespace BrownBeltWeek2{

#pragma region Loading data from XML

  struct Spending {
    std::string category;
    int amount;
  };

  std::vector<Spending> LoadFromXml(std::istream& input) {
    std::vector<Spending> result;
    Xml::Document doc = Xml::Load(input);
    const Xml::Node& root = doc.GetRoot();
    for (const auto& child : root.Children()) {
      result.push_back({
        child.AttributeValue<std::string>("category"),
        child.AttributeValue<int>("amount") });
    }
    return result;
  }

#pragma endregion

#pragma region Loading data from JSON

  std::vector<Spending> LoadFromJson(std::istream& input);

#pragma endregion


#pragma region Code refactoring

  Json::Document XmlToJson(const Xml::Document& doc);

  Xml::Document JsonToXml(const Json::Document& doc, std::string root_name);

#pragma endregion



}