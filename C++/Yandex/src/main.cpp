#include <time_logger.h>
#include <unit_test.h>

// using namespace std;
using namespace unit_test;
using namespace log_time;

#include <memory>
#include <vector>

#include "3_brown_belt\2_NamespacesAndThisPtr\geo\game_object.h"
#include "3_brown_belt\2_NamespacesAndThisPtr\geo\geo2d.h"

using namespace std;

class Unit : public GameObject {
  public:
  explicit Unit(geo2d::Point position) : d_pos(position) {}

  geo2d::Point getPos() const
  {
    return d_pos;
  }

  bool Collide(const GameObject& that) const
  {
    return that.CollideWith(*this);
  }

  bool CollideWith(const Unit& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Building& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Tower& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Fence& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }

  private:
  geo2d::Point d_pos;
};

class Building : public GameObject {
  public:
  explicit Building(geo2d::Rectangle geometry) : d_pos(geometry) {}

  geo2d::Rectangle getPos() const
  {
    return d_pos;
  }

  bool Collide(const GameObject& that) const
  {
    return that.CollideWith(*this);
  }

  bool CollideWith(const Unit& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Building& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Tower& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Fence& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }

  private:
  geo2d::Rectangle d_pos;
};

class Tower : public GameObject {
  public:
  explicit Tower(geo2d::Circle geometry) : d_pos(geometry) {}

  geo2d::Circle getPos() const
  {
    return d_pos;
  }

  bool Collide(const GameObject& that) const
  {
    return that.CollideWith(*this);
  }

  bool CollideWith(const Unit& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Building& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Tower& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Fence& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }

  private:
  geo2d::Circle d_pos;
};

class Fence : public GameObject {
  public:
  explicit Fence(geo2d::Segment geometry) : d_pos(geometry) {}

  geo2d::Segment getPos() const
  {
    return d_pos;
  }

  bool Collide(const GameObject& that) const
  {
    return that.CollideWith(*this);
  }

  bool CollideWith(const Unit& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Building& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Tower& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }
  bool CollideWith(const Fence& that) const override
  {
    return geo2d::Collide(d_pos, that.getPos());
  }

  private:
  geo2d::Segment d_pos;
};

bool Collide(const GameObject& first, const GameObject& second)
{
  return first.Collide(second);
}

void TestAddingNewObjectOnMap()
{
  using namespace geo2d;

  vector<shared_ptr<GameObject>> game_map = {
    std::make_shared<Unit>(Point{ 3, 3 }),
    std::make_shared<Unit>(Point{ 5, 5 }),
    std::make_shared<Unit>(Point{ 3, 7 }),
    std::make_shared<Fence>(Segment{ { 7, 3 }, { 9, 8 } }),
    std::make_shared<Tower>(Circle{ Point{ 9, 4 }, 1 }),
    std::make_shared<Tower>(Circle{ Point{ 10, 7 }, 1 }),
    std::make_shared<Building>(Rectangle{ { 11, 4 }, { 14, 6 } })
  };

  for (size_t i = 0; i < game_map.size(); ++i)
  {
    Assert(Collide(*game_map[i], *game_map[i]),
           "An object doesn't collide with itself: " + to_string(i));

    for (size_t j = 0; j < i; ++j)
    {
      Assert(!Collide(*game_map[i], *game_map[j]),
             "Unexpected collision found " + to_string(i) + ' ' + to_string(j));
    }
  }

  auto new_warehouse = make_shared<Building>(Rectangle{ { 4, 3 }, { 9, 6 } });
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT(Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT(Collide(*new_warehouse, *game_map[3]));
  ASSERT(Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{ { 8, 2 }, 2 });
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT(Collide(*new_defense_tower, *game_map[3]));
  ASSERT(Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestAddingNewObjectOnMap);
  return 0;
}
