
#include "week_5.h"

#include <sstream>

using namespace std;

const size_t MaxRawsInLine = 10'000u;
const size_t MaxWordLength = 20;

namespace RedBeltW5_NS
{
  using FuturesVec = std::vector<std::future<Stats>>;
  using StreamVec = std::vector<stringstream>;

  void Stats::operator+=(const Stats& i_other)
  {
    for (const auto& [word, count] : i_other.word_frequences)
      word_frequences[word] += count;
  }

  Stats ExploreLine(const set<string>& key_words, const std::string& line)
  {
    if (line.empty()) return {};

    Stats res;
    size_t wordLength{ 0u }, beginId{ 0u }, endId{ 0u };
    bool isLastWord{ false };

    std::string curWord;
    curWord.reserve(MaxWordLength);

    for (; !isLastWord && beginId < line.size();)
    {
      endId = line.find(' ', beginId);
      if (endId == std::string::npos) isLastWord = true;

      wordLength = endId - beginId;
      if (wordLength >= 1)
      {
        curWord = line.substr(beginId, wordLength);
        if (key_words.find(curWord) != key_words.end())
          ++res.word_frequences[curWord];
      }
      // Go to next symbol in a line
      beginId = endId + 1;
    }

    return res;
  }

  Stats ExploreKeyWordsSingleThread(const set<string>& key_words,
                                    istream& input)
  {
    Stats result;
    for (string line; getline(input, line);)
    {
      result += ExploreLine(key_words, line);
    }
    return result;
  }

  Stats ExploreKeyWords(const set<string>& key_words, istream& input)
  {
    Stats result;

    FuturesVec futures;
    StreamVec ssvector;
    std::string line;
    std::stringstream ss;

    ssvector.reserve(1'000);
    line.reserve(1'000);

    while (!input.eof())
    {
      for (size_t lineId = 0; lineId != MaxRawsInLine; ++lineId)
      {
        std::getline(input, line);
        ss << line << '\n';
      }
      ssvector.push_back(move(ss));
    }

    for (auto& curSs : ssvector)
      futures.push_back(
          std::async(ExploreKeyWordsSingleThread, ref(key_words), ref(curSs)));

    for (auto& f : futures)
      result += f.get();

    return result;
  }

}  // namespace RedBeltW5_NS
