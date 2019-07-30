#include "AssociativeContainers.h"

namespace BrownBeltWeek1 {

  Node::Node(int v, Node* p) : value(v), parent(p) {}

  Node* Successor(Node* io_rigthNode) {
    Node* curNode = io_rigthNode;
    while (curNode->left) {
      curNode = curNode->left;
    }
    return curNode;
  }

  bool isLeaf(Node* i_curNode) {
    return !i_curNode->left && !i_curNode->right;
  }

  Node* Next(Node* node) {
    Node* curNode = node;

    // Check if root only
    if (!node->parent && !node->left && !node->right)
      return nullptr;

    if (isLeaf(node)) {
      Node* parentNode = node->parent;
      if (node == parentNode->left)
        return parentNode;

      if (Node* grParentNode = parentNode->parent; grParentNode)
      {
        while (parentNode == grParentNode->right) {
          parentNode = grParentNode;
          // If on the "right way back": this node contains max element
          if (!parentNode->parent)
            return nullptr;
          grParentNode = parentNode->parent;
          if (!grParentNode)
            return node;
        }

        if (parentNode == grParentNode->left)
          return grParentNode;
      }
      else return node;
    }

    if (curNode->right)
      return Successor(curNode->right);
  }

  bool Point3D::operator==(const Point3D& i_other) const {
    return std::make_tuple(x, y, z) == std::make_tuple(i_other.x, i_other.y, i_other.z);
  }

  size_t Hasher::operator()(const Point3D& i_point) const {
    size_t a = d_hash(i_point.x);
    size_t b = d_hash(i_point.y);
    size_t c = d_hash(i_point.z);

    return (((((31 + a) * 37) + b) * 41) + c) * 43;
  }

  bool Address::operator==(const Address& other) const {
    return std::make_tuple(city, street, building) == std::make_tuple(other.city, other.street, other.building);
  }

  bool Person::operator==(const Person& other) const {
    return std::make_tuple(name, height, weight, address) == std::make_tuple(name, height, weight, address);
  }

  size_t AddressHasher::operator()(const Address& i_address) const {
    size_t a = d_hashStr(i_address.city);
    size_t b = d_hashStr(i_address.street);
    size_t c = d_hashInt(i_address.building);

    return (((((31 + a) + 37) * b) + 43) * c) + 997;
  }

  size_t PersonHasher::operator()(const Person& i_person) const {
    size_t a = d_hashStr(i_person.name);
    size_t b = d_hashInt(i_person.height);
    size_t c = d_hashDouble(i_person.weight);
    size_t d = d_hashAddr(i_person.address);

    return a * PrimVal * PrimVal * PrimVal + b * PrimVal * PrimVal + c * PrimVal + d;
  }

  bool Database::Put(const Record& record) {
    if (auto idIter = d_body.find(record.id); idIter == d_body.end()) {
      auto iterBoolPair = d_body.insert({ record.id, record });

      auto timeIter = d_timestepMap.insert({ record.timestamp, iterBoolPair.first });
      auto karmaIter = d_karmaMap.insert({ record.karma, iterBoolPair.first });
      auto userIter = d_userMap.insert({ record.user, iterBoolPair.first });

      d_iterMap.insert({ record.id, {timeIter, karmaIter, userIter} });
      return true;
    }
    return false;
  }

  const Record* Database::GetById(const std::string& id) const {
    if (auto idIter = d_body.find(id); idIter != d_body.end()) {
      return &idIter->second;
    }
    return nullptr;
  }

  bool Database::Erase(const std::string& id) {
    if (auto idIter = d_body.find(id); idIter != d_body.end()) {
      const auto iters = d_iterMap.at(id);

      d_timestepMap.erase(std::get<0>(iters));
      d_karmaMap.erase(std::get<1>(iters));
      d_userMap.erase(std::get<2>(iters));

      d_body.erase(id);
      return true;
    }
    return false;
  }


}

  