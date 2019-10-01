#pragma once

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

namespace Server_NS
{
  class InvertedIndex {
    public:
    void Add(const std::string& i_document);
    std::list<size_t> Lookup(const std::string& i_word) const;

    const std::string& GetDocument(size_t i_id) const
    {
      return d_docs[i_id];
    }

    private:
    std::map<std::string, std::list<size_t>> d_index;
    std::vector<std::string> d_docs;
  };

  class SearchServer {
    public:
    SearchServer() = default;
    explicit SearchServer(std::istream& i_documentInput);
    void UpdateDocumentBase(std::istream& i_documentInput);
    void AddQueriesStream(std::istream& i_queryInput,
                          std::ostream& i_searchResultsOutput);

    private:
    InvertedIndex d_index;
  };
}  // namespace Server_NS
