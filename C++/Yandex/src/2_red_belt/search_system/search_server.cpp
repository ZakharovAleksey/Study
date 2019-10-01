#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "iterator_range.h"

using namespace std;

namespace Server_NS
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
    for (string current_query; getline(i_queryInput, current_query);)
    {
      const auto words = SplitIntoWords(current_query);

      map<size_t, size_t> docid_count;
      for (const auto& word : words)
      {
        for (const size_t docid : d_index.Lookup(word))
        {
          docid_count[docid]++;
        }
      }

      vector<pair<size_t, size_t>> search_results(docid_count.begin(),
                                                  docid_count.end());
      sort(begin(search_results), end(search_results),
           [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
             int64_t lhs_docid = lhs.first;
             auto lhs_hit_count = lhs.second;
             int64_t rhs_docid = rhs.first;
             auto rhs_hit_count = rhs.second;
             return make_pair(lhs_hit_count, -lhs_docid) >
                    make_pair(rhs_hit_count, -rhs_docid);
           });

      i_searchResultsOutput << current_query << ':';
      for (auto [docid, hitcount] : Head(search_results, 5))
      {
        i_searchResultsOutput << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      i_searchResultsOutput << endl;
    }
  }

  void InvertedIndex::Add(const string& i_document)
  {
    d_docs.push_back(i_document);

    const size_t docid = d_docs.size() - 1;
    for (const auto& word : SplitIntoWords(i_document))
    {
      d_index[word].push_back(docid);
    }
  }

  list<size_t> InvertedIndex::Lookup(const string& i_word) const
  {
    if (auto it = d_index.find(i_word); it != d_index.end())
    {
      return it->second;
    } else
    {
      return {};
    }
  }

}  // namespace Server_NS
