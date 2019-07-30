#pragma once 

#include"xml.h"
namespace BrownBeltWeek2{

#pragma region Loading data from XML

  struct Spending {
    string category;
    int amount;
  };

  vector<Spending> LoadFromXml(istream& input) {
    vector<Spending> result;
    Document doc = Load(input);
    const Node& root = doc.GetRoot();
    for (const auto& child : root.Children()) {
      result.push_back({
        child.AttributeValue<std::string>("category"),
        child.AttributeValue<int>("amount") });
    }
    return result;
  }

#pragma endregion

}