#include"ini.h"

namespace Ini {

  Section& Document::AddSection(std::string name) {
    return sections[name];
  }

  const Section& Document::GetSection(const std::string& name) const {
    return sections.at(name);
  }

  size_t Document::SectionCount() const {
    return sections.size();
  }

  stringPair Split(const std::string& i_line, char i_char) {
    const size_t splitId = i_line.find('=');
    return { i_line.substr(0, splitId), i_line.substr(splitId + 1, i_line.size()) };
  }

  Document Load(std::istream& input) {
    Document doc;

    std::string line;
    std::getline(input, line);
    if(line.empty())
      return doc;
    Ini::Section* section = &doc.AddSection(line.substr(1, line.size() - 2));

    for (;std::getline(input, line);) {
      if(line.empty()) continue;
      else {
        if(line.front() == '[')
          section = &doc.AddSection(line.substr(1, line.size() - 2));
        else 
          section->insert(Split(line, '='));
      }
    }
    return doc;
  }
}