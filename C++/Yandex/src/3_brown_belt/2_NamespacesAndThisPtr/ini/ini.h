#pragma once

#include <unordered_map>
#include <cstring>
#include <string>
#include <istream>

namespace Ini {
  using Section = std::unordered_map<std::string, std::string>;

  class Document {
  public:
    Section& AddSection(std::string name);
    const Section& GetSection(const std::string& name) const;
    size_t SectionCount() const;

  private:
    std::unordered_map<std::string, Section> sections;
  };

  using stringPair = std::pair<std::string, std::string>;

  stringPair Split(const std::string& i_line, char i_char);
  Document Load(std::istream& input);
}