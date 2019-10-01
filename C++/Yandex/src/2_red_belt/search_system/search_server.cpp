#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "iterator_range.h"

using namespace std;

namespace Server_NS
{
  const size_t MaxQueryWords = 10u;

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
    InvertedIndex newIndex;

    for (string curDoc; getline(i_documentInput, curDoc);)
    {
      newIndex.Add(move(curDoc));
    }

    d_index = move(newIndex);
  }

  void SearchServer::AddQueriesStream(istream& i_queryInput,
                                      ostream& i_searchResultsOutput)
  {
    std::vector<std::string> words;
    words.reserve(MaxQueryWords);

    for (string current_query; getline(i_queryInput, current_query);)
    {
      words = SplitIntoWords(current_query);

      std::vector<int> docsImportance(d_index.getDocumentCount());

      for (const auto& word : words)
        for (const size_t docId : d_index.Lookup(word))
          ++docsImportance[docId];

      size_t topCount = std::min(static_cast<int>(docsImportance.size()), 5);

      i_searchResultsOutput << current_query << ':';
      for (size_t id = 0; id < topCount; ++id)
      {
        const auto hitCountIter =
            std::max_element(docsImportance.begin(), docsImportance.end());

        // If there is no more existence of this word in sentence
        if (*hitCountIter == 0)
          break;

        i_searchResultsOutput
            << " {"
            << "docid: " << std::distance(docsImportance.begin(), hitCountIter)
            << ", "
            << "hitcount: " << *hitCountIter << '}';

        *hitCountIter = -1;
      }
      i_searchResultsOutput << endl;
    }
  }

  InvertedIndex::InvertedIndex()
  {
    d_index.reserve(MaxWordsCount);
  }

  void InvertedIndex::Add(const string& i_document)
  {
    d_docs[d_docsCount] = i_document;

    for (const auto& word : SplitIntoWords(i_document))
    {
      d_index[word].emplace_back(d_docsCount);
    }
    ++d_docsCount;
  }

  std::vector<DocId> InvertedIndex::Lookup(const string& i_word) const
  {
    if (auto it = d_index.find(i_word); it != d_index.end())
      return it->second;
    else
      return {};
  }

  size_t InvertedIndex::getDocumentCount() const
  {
    return d_docsCount;
  }

}  // namespace Server_NS
