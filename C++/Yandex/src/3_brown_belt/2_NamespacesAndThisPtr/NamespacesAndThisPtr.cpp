#include "NamespacesAndThisPtr.h"

namespace BrownBeltWeek2
{
  std::vector<Spending> LoadFromJson(std::istream& input)
  {
    std::vector<Spending> result;
    Json::Document doc = Json::Load(input);
    for (const auto& element : doc.GetRoot().AsArray())
    {
      const std::map<std::string, Json::Node>& node = element.AsMap();
      result.push_back(
          { node.at("category").AsString(), node.at("amount").AsInt() });
    }
    return result;
  }

  Json::Document XmlToJson(const Xml::Document& doc)
  {
    using namespace std;
    vector<Json::Node> result;

    for (const Xml::Node& n : doc.GetRoot().Children())
    {
      Json::Node catNode(n.AttributeValue<string>("category"));
      Json::Node amountNode(n.AttributeValue<int>("amount"));

      result.emplace_back(
          map<string, Json::Node>{ { "category", std::move(catNode) },
                                   { "amount", std::move(amountNode) } });
    }

    return Json::Document(Json::Node(std::move(result)));
  }

  Xml::Document JsonToXml(const Json::Document& doc, std::string root_name)
  {
    using namespace std;

    Xml::Node root(move(root_name), {});
    for (const auto& n : doc.GetRoot().AsArray())
    {
      string catStr = n.AsMap().at("category").AsString();
      string attrStr = std::to_string(n.AsMap().at("amount").AsInt());

      root.AddChild(Xml::Node("spend", { { "category", std::move(catStr) },
                                         { "amount", std::move(attrStr) } }));
    }

    return Xml::Document(root);
  }

#pragma region Dual scheduling

  bool Collide(const GameObject& first, const GameObject& second)
  {
    return first.Collide(second);
  }

#define DEFINE_METHOD_COLLIDE_WITH(First, Second)

#define DEFINE_METHODS(Type)                                                   \
  bool Type::CollideWith(const Unit& that) const                               \
  {                                                                            \
    return geo2d::Collide(d_pos, that.getPos());                               \
  }                                                                            \
  bool Type::CollideWith(const Building& that) const                           \
  {                                                                            \
    return geo2d::Collide(d_pos, that.getPos());                               \
  }                                                                            \
  bool Type::CollideWith(const Tower& that) const                              \
  {                                                                            \
    return geo2d::Collide(d_pos, that.getPos());                               \
  }                                                                            \
  bool Type::CollideWith(const Fence& that) const                              \
  {                                                                            \
    return geo2d::Collide(d_pos, that.getPos());                               \
  }

  // DEFINE_METHODS(Unit);
#pragma endregion

}  // namespace BrownBeltWeek2
