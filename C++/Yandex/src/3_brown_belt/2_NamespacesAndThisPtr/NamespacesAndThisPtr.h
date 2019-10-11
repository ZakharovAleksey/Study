#pragma once

#include "geo/game_object.h"
#include "geo/geo2d.h"
#include "json/json.h"
#include "xml/xml.h"

namespace BrownBeltWeek2
{
#pragma region Loading data from XML

  struct Spending
  {
    std::string category;
    int amount;
  };

  std::vector<Spending> LoadFromXml(std::istream& input)
  {
    std::vector<Spending> result;
    Xml::Document doc = Xml::Load(input);
    const Xml::Node& root = doc.GetRoot();
    for (const auto& child : root.Children())
    {
      result.push_back({ child.AttributeValue<std::string>("category"),
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

#pragma region Dual scheduling

  //  // Macroses
  //
  //#define DECLARE_METHODS \
//  bool CollideWith(const Unit& that) const override; \
//  bool CollideWith(const Building& that) const; \
//  bool CollideWith(const Tower& that) const; \ bool CollideWith(const Fence&
  //  that) const;
  //
  //  // CRTP Idiom:
  //  // https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
  //
  //  template<typename T>
  //  struct Collider : GameObject
  //  {
  //    bool Collide(const GameObject& i_that) const final
  //    {
  //      return i_that.CollideWith(static_cast<const T&>(*this));
  //    }
  //  };
  //
  //  class Unit : public Collider<Unit> {
  //    public:
  //    explicit Unit(geo2d::Point position) : d_pos(position) {}
  //
  //    geo2d::Point getPos() const
  //    {
  //      return d_pos;
  //    }
  //
  //    DECLARE_METHODS
  //
  //    private:
  //    geo2d::Point d_pos;
  //  };
  //
  //  class Building : public Collider<Building> {
  //    public:
  //    explicit Building(geo2d::Rectangle geometry) : d_pos(geometry) {}
  //
  //    geo2d::Rectangle getPos() const
  //    {
  //      return d_pos;
  //    }
  //
  //    // Uncomment this
  //    DECLARE_METHODS
  //
  //    private:
  //    geo2d::Rectangle d_pos;
  //  };
  //
  //  class Tower : public Collider<Tower> {
  //    public:
  //    explicit Tower(geo2d::Circle geometry) : d_pos(geometry) {}
  //
  //    geo2d::Circle getPos() const
  //    {
  //      return d_pos;
  //    }
  //
  //    // Uncomment this
  //    DECLARE_METHODS
  //
  //    private:
  //    geo2d::Circle d_pos;
  //  };
  //
  //  class Fence : public Collider<Fence> {
  //    public:
  //    explicit Fence(geo2d::Segment geometry) : d_pos(geometry) {}
  //
  //    geo2d::Segment getPos() const
  //    {
  //      return d_pos;
  //    }
  //
  //    // Uncomment this
  //    DECLARE_METHODS
  //
  //    private:
  //    geo2d::Segment d_pos;
  //  };
  //
  //  bool Collide(const GameObject& first, const GameObject& second);

#pragma endregion

}  // namespace BrownBeltWeek2
