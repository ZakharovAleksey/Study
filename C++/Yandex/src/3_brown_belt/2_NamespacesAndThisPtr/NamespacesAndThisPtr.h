#pragma once 

#include"xml.h"
#include"json.h"

namespace BrownBeltWeek2{

#pragma region Loading data from XML

  struct Spending {
    std::string category;
    int amount;
  };

  std::vector<Spending> LoadFromXml(istream& input) {
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

  std::vector<Spending> LoadFromJson(istream& input) {
    std::vector<Spending> result;
    Json::Document doc = Json::Load(input);
    for (const auto& element : doc.GetRoot().AsArray()) {
      const map<string, Json::Node>& node = element.AsMap();
      result.push_back(
        { node.at("category").AsString(), node.at("amount").AsInt() }
      );
    }
    return result;
  }

#pragma endregion


}