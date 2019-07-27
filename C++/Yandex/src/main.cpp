#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

#include <cstdint>
#include<memory>
#include<array>

#pragma region PQ

template <typename T>
class PriorityCollection {
public:
  using Priority = int;
  using Id = size_t;
  using PriorIdPair = pair<Priority, Id>;

  static const size_t N = 1'000;

  PriorityCollection() : d_curId(0) {
    d_objects.reserve(N);
    std::fill(d_isObjValid.begin(), d_isObjValid.end(), false);
    std::fill(d_objPriority.begin(), d_objPriority.end(), 0);
  }

  Id Add(T object) {
    d_objects.push_back(move(object));
    d_isObjValid[d_curId] = true;
    d_heap.insert({0, d_curId});
    // Default priority is 0 (no assigment is needed)
    return d_curId++;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto iter = range_begin; iter != range_end; ++iter) {
      auto curId{ 0 };// = Add(move(*iter));
      *(ids_begin++) = curId;
    }
  }

  bool IsValid(Id id) const {
    return d_isObjValid[id];
  }

  const T& Get(Id id) const {
    return move(d_objects[id]);
  }

  void Promote(Id id) {
    PriorIdPair pair = make_pair(d_objPriority[id], id);

    auto iter = d_heap.find(pair);
    if (iter != d_heap.end()) {
      PriorIdPair newPair = { (iter->first) + 1, iter->second };
      d_heap.erase(pair);
      d_heap.insert(newPair);
      d_objPriority[id] = newPair.first;
    }
  }

  pair<const T&, int> GetMax() const {
    return { d_objects[d_heap.begin()->second], d_heap.begin()->first };
  }

  pair<T, int> PopMax() {
    d_isObjValid[d_heap.begin()->second] = false;

    pair<T, int> res{ move(d_objects[d_heap.begin()->second]), d_heap.begin()->first };
    d_heap.erase(d_heap.begin());
    return res;
  }

  struct SetComparator {
    bool operator()(const PriorIdPair& left, const PriorIdPair& right) const {
      if (left.first > right.first) {
        return true;
      }
      else if (left.first == right.first) {
        return left.second > right.second;
      }
      return false;
    }

  };

private:
  size_t d_curId;
  vector<T> d_objects;
  array<bool, N> d_isObjValid;
  array<Priority, N> d_objPriority;
  set<PriorIdPair, SetComparator> d_heap;
};


class StringNonCopyable : public string {
public:
  using string::string;
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

void TestNoCopyRange() {
  size_t len{ 3 };
  PriorityCollection<StringNonCopyable> strings;

  vector<StringNonCopyable> input;
  input.push_back({ "white" });
  input.push_back({ "yellow" });
  input.push_back({ "red" });
  
  vector<int> ids; 
  ids.reserve(len);

  strings.Add(input.begin(), input.end(), ids.begin());

  strings.Promote(ids[1]);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(ids[2]);
  }
  strings.Promote(ids[1]);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

using Priority = int;
using Id = size_t;

using PriorityIdPair = std::pair<Priority, Id>;

struct MyPairComparator {
  bool operator()(const PriorityIdPair& left, const PriorityIdPair& right) {
    if (left.first > right.first) {
      return true;
    }
    else if (left.first == right.first) {
      return left.second < right.second;
    }
    return false;
  }
};

ostream& operator<<(ostream& os, const PriorityIdPair& p) {
  os << "(" << p.first << ", " << p.second << ") ";
  return os;
}

#pragma endregion


#pragma region ASUNC

#include<future>
#include<numeric>

//#include"2_red_belt\week_1\week_1.h"
//
//struct Stats {
//  map<string, int> word_frequences;
//
//  void insert(const string& key, int numb) {
//    word_frequences[key] = numb;
//  }
//
//  friend std::ostream & operator<<(std::ostream & os, const Stats & s) {
//    os << s.word_frequences << endl;
//    return os;
//  }
//
//  void operator += (const Stats& other) {
//    for (auto&[key, value] : other.word_frequences) {
//      if (auto it = word_frequences.find(key); it != word_frequences.end()) {
//        it->second += value;
//      }
//      else
//        word_frequences[key] = value;
//    }
//  }
//};
//
//bool isSeparateWord(const size_t i_curPos, const string& key, const string& line){
//  auto curIt = begin(line) + i_curPos;
//  auto befKeyIt = (curIt == begin(line)) ? begin(line) : prev(curIt);
//  auto aftKeyIt = (i_curPos + key.size() > line.size()) ? end(line) : curIt + key.size();
//
//  return (befKeyIt == begin(line) || *befKeyIt == ' ') && (aftKeyIt == end(line) || *aftKeyIt == ' ');
//}
//
//size_t countKeyEntries(const string& key, const string& line){
//  size_t curNumb{ 0u }, curPos{ 0u };
//
//  while (true) {
//    curPos = line.find(key, curPos);
//
//    if (curPos != std::string::npos) {
//      if (isSeparateWord(curPos, key, line))
//        ++curNumb;
//      ++curPos;
//    }
//    else
//      break;
//  }
//  return curNumb;
//}
//
//Stats ExploreLine(const set<string>& key_words, const string& line) {
//  Stats stat;
//
//  for (const auto& key : key_words) {
//    int curNumb = countKeyEntries(key, line);
//    if(curNumb != 0u)
//      stat.insert(key, curNumb);
//  }
//
//  return stat;
//}
//
//Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
//    Stats result;
//    for (string line; getline(input, line); ) {
//      result += ExploreLine(key_words, line);
//    }
//    return result;
//  }
////
////Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
////  Stats result;
////  std::vector<std::future<Stats>> futures;
////  std::vector<stringstream> ssvector; ssvector.reserve(1'000);
////  std::string line; line.reserve(1'000'000);
////  stringstream ss;
////
////  while (!input.eof()) {
////    for(size_t lineId = 0; lineId != 3; ++lineId){
////      std::getline(input, line);
////      ss << line << '\n';
////    }
////    ssvector.push_back(move(ss));
////  }
////
////  for(auto& curSs : ssvector){
////    futures.push_back(
////      async(ExploreKeyWordsSingleThread, ref(key_words), ref(curSs))
////    );
////  }
////
////  for(auto& f : futures){
////    result += f.get();
////  }
////
////  return result;
////}
////
//
//Stats ExploreKeyWords(const set<string>& key_words, istream& input)
//{
//  vector<future<Stats>> stats;
//  vector<istringstream> streams;
//  streams.reserve(20);
//  const size_t CHARS_IN_STREAM = 1000000;
//  string tail;
//
//  string lines;
//  lines.reserve(CHARS_IN_STREAM);
//  while (!input.eof())
//  {
//    copy(tail.begin(), tail.end(), back_inserter(lines));
//    copy_n(istreambuf_iterator(input), CHARS_IN_STREAM - tail.size(), back_inserter(lines));
//
//    auto newline_pos = lines.find_last_of('\n');
//    if (newline_pos < lines.size() - 1)
//    {
//      tail.assign(lines.substr(newline_pos + 1));
//      lines.resize(newline_pos + 1);
//    }
//
//    streams.emplace_back(lines);
//    stats.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(streams[streams.size() - 1])));
//    lines.clear();
//  }
//
//  Stats res{};
//  for (auto & stat : stats)
//  {
//    res += stat.get();
//  }
//
//  return res;
//}


#pragma endregion

#include"2_red_belt\week_1\week_1.h"

struct Stats {
  map<string, int> word_frequences;

  void insert(const string& key, const int value){
    word_frequences[key] = value;
  }

  void operator += (const Stats& other){
    for(const auto& [key, value]: other.word_frequences)
      word_frequences[key] += value;
  }
};

bool isKey(const int i_pos, const string& i_key, const string& i_line){
  if(i_pos == 0 || i_pos + i_key.size() == i_line.size()) 
    return true;
  return i_line[i_pos - 1] == ' ' && i_line[i_pos + i_key.size()] == ' ';
}

template<typename ContainerOfVectors>
Stats ExploreLineSingleThread(const ContainerOfVectors& key_words, const string& line)
{
  Stats res;
  size_t curPos{ 0u }, count{ 0u };
  for (const auto& key : key_words) {
    curPos = 0u; count = 0u;
    while (true) {
      curPos = line.find(key, curPos);
      if (curPos == std::string::npos)
        break;
      if (isKey(curPos, key, line))
        ++count;
      curPos += key.size();
    }
    if (count != 0)
      res.word_frequences[key] = count;
  }
  return res;
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
  Stats res;
 
  for(auto p : week_1::Paginate(key_words, key_words.size() / 4))
    res += std::async([p, &line] { return ExploreLineSingleThread(p, line); }).get();

  return res;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

std::pair<string, int> calc(const std::vector<string>& v, const string& key){
  return {key, std::count(begin(v), end(v), key)};
}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//  Stats result;
//  std::vector<std::future<std::pair<string, int>>> futures;
//  std::vector<std::string> lol; lol.reserve(1'000'000);
//  
//  std::copy(std::istream_iterator<string>(input), 
//            std::istream_iterator<string>(), 
//            std::back_inserter(lol)
//  );
//  
//  for(const auto& key : key_words){
//    auto p = futures.emplace_back(
//      async(calc, ref(lol), ref(key))
//    ).get();
//    if (p.second != 0)
//      result.insert(p.first, p.second);
//  }
//  
//  return result;
//}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  Stats result;
  std::vector<stringstream> ssvector; ssvector.reserve(20);
  std::string line; line.reserve(1'000);
  stringstream ss;

  while (!input.eof()) {
    for (size_t lineId = 0; lineId != 10'000; ++lineId) {
      std::getline(input, line);
      ss << line << '\n';
    }
    ssvector.push_back(move(ss));
  }

  for (auto& curSs : ssvector)
    result += async(ExploreKeyWordsSingleThread, ref(key_words), ref(curSs)).get();

  return result;
}



int main() {
  TestRunner tr;
  
  Stats a; a.word_frequences = {{"lol", 1}, {"not", 2}};
  Stats b; b.word_frequences = { {"lol", 3}, {"not_lol", 4} };

  a+=b;

  auto r = ExploreLine({ "mom", "lol", "zah" }, { "mom and lol are inlol and lolmom not lol" });

  int c = 0;


  
  ////auto r = ExploreLine({ "mom", "lol" }, { "  " });
  //


  const set<string> key_words = { "yangle", "rocks", "sucks", "all" };
  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);

  int lol;
  //



  return 0;
}