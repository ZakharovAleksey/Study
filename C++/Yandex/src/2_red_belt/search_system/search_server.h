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

namespace Server_NS
{
  // 10'000
  const size_t MaxWordsCount = 100;
  // 50'000
  const size_t MaxDocsCount = 500;

  using Word = std::string;
  using DocId = size_t;

  using WordToDocIdsMap = std::unordered_map<Word, std::vector<DocId>>;

  class InvertedIndex {
    public:
    InvertedIndex();
    void Add(const std::string& i_document);
    std::vector<DocId> Lookup(const std::string& i_word) const;

    const std::string& GetDocument(size_t i_id) const
    {
      return d_docs[i_id];
    }

    size_t getDocumentCount() const;

    private:
    WordToDocIdsMap d_index;
    std::array<Word, MaxDocsCount> d_docs;
    DocId d_docsCount{ 0u };
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
