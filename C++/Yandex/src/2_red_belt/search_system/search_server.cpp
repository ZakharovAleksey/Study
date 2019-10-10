#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "iterator_range.h"

using namespace std;

namespace RedBeltFinal_NS
{
  vector<string> SplitIntoWords(const string& i_line)
  {
    istringstream words_input(i_line);
    return { istream_iterator<string>(words_input),
             istream_iterator<string>() };
  }

  SearchServer::SearchServer(istream& i_documentInput)
  {
    UpdateDocumentBase(i_documentInput);
  }

  void SearchServer::UpdateDocumentBase(istream& i_documentInput)
  {
    InvertedIndex new_index;

    for (string current_document; getline(i_documentInput, current_document);)
    {
      new_index.Add(move(current_document));
    }

    d_index = move(new_index);
  }

  void SearchServer::AddQueriesStream(istream& i_queryInput,
                                      ostream& i_searchResultsOutput)
  {
    std::vector<std::pair<size_t, size_t>> docCount(
        d_index.getDocumentsCount());

    for (string current_query; getline(i_queryInput, current_query);)
    {
      const auto words = SplitIntoWords(current_query);

      // Clear array
      for (size_t i = 0; i < docCount.size(); ++i)
      {
        docCount[i].first = i;
        docCount[i].second = 0u;
      }

      for (const auto& word : words)
        for (const auto& [docId, wordCount] : d_index.Lookup(word))
          docCount[docId].second += wordCount;

      const size_t numb = std::min(static_cast<int>(docCount.size()), 5);

      std::partial_sort(std::begin(docCount), std::begin(docCount) + numb,
                        std::end(docCount),
                        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                          int64_t lhs_docid = lhs.first;
                          auto lhs_hit_count = lhs.second;
                          int64_t rhs_docid = rhs.first;
                          auto rhs_hit_count = rhs.second;
                          return make_pair(lhs_hit_count, -lhs_docid) >
                                 make_pair(rhs_hit_count, -rhs_docid);
                        });

      i_searchResultsOutput << current_query << ':';
      for (auto [docid, hitcount] : Head(docCount, 5))
      {
        if (hitcount == 0) break;
        i_searchResultsOutput << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      i_searchResultsOutput << endl;
    }
  }

  void InvertedIndex::Add(const string& i_document)
  {
    d_docs[d_docCount] = i_document;

    for (const auto& word : SplitIntoWords(i_document))
      ++d_index[word][d_docCount];

    ++d_docCount;
  }

  std::map<DocId, size_t> InvertedIndex::Lookup(const string& i_word) const
  {
    if (auto it = d_index.find(i_word); it != d_index.end())
      return it->second;
    else
      return {};
  }

}  // namespace RedBeltFinal_NS
