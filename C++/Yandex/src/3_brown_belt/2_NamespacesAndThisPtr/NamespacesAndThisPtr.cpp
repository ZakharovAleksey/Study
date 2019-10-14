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

  /*bool Collide(const GameObject& first, const GameObject& second)
  {
    return first.Collide(second);
  }

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

  DEFINE_METHODS(Unit);
  DEFINE_METHODS(Building);
  DEFINE_METHODS(Tower);
  DEFINE_METHODS(Fence);*/

#pragma endregion

  HttpResponse& HttpResponse::AddHeader(std::string i_name, std::string i_value)
  {
    d_headers.insert({ std::move(i_name), std::move(i_value) });
    return *this;
  }

  HttpResponse& HttpResponse::SetContent(std::string i_content)
  {
    d_content = std::move(i_content);

    if (!d_content.empty())
    {
      // There can be only one "Content-Length" header
      if (auto iter = d_headers.find("Content-Length"); iter != d_headers.end())
        iter->second = std::to_string(d_content.size());
      else
        // return AddHeader("Content-Length", to_string(d_content.size()));
        d_headers.insert(
            { "Content-Length", std::to_string(d_content.size()) });
    }
    return *this;
  }
  HttpResponse& HttpResponse::SetCode(HttpCode i_code)
  {
    d_code = i_code;
    return *this;
  }

  std::ostream& operator<<(std::ostream& io_os, const HttpResponse& i_resp)
  {
    // Print the top line
    io_os << "HTTP/1.1 " << static_cast<int>(i_resp.d_code) << ' '
          << HttpResponse::HttpComment.at(i_resp.d_code) << '\n';

    // Print all headers
    for (const auto& [header, value] : i_resp.d_headers)
      io_os << header << ": " << value << '\n';

    // Skip the line
    io_os << '\n';

    // Print the content
    io_os << i_resp.d_content;

    return io_os;
  }

}  // namespace BrownBeltWeek2
