#pragma once

#include <array>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace RedBeltFinal_NS
{
  const size_t MaxWordsCount = 500u;
  using DocId = size_t;

  class InvertedIndex {
    public:
    using Word = std::string;
    using DocId = size_t;

    using WordToDocIdsMap = std::unordered_map<Word, std::vector<DocId>>;

    InvertedIndex();
    void Add(const std::string& i_document);
    std::map<DocId, size_t> Lookup(const std::string& i_word) const;

    const std::string& GetDocument(size_t i_id) const
    {
      return d_docs[i_id];
    }

    size_t getDocumentsCount() const
    {
      return d_docCount;
    }

    private:
    std::unordered_map<Word, std::map<DocId, size_t>> d_index;
    std::array<Word, MaxWordsCount> d_docs;
    size_t d_docCount{ 0u };
  };  // namespace RedBeltFinal_NS

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

}  // namespace RedBeltFinal_NS
