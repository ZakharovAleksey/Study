#pragma once

#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<iterator>
#include<future>
#include<numeric>

#include"../week_1/week_1.h"

namespace RedBeltW5 {

	using namespace std;

	// Task 1. Josephus permutation without copy
	template<typename RandomIt>
	void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
		vector<typename RandomIt::value_type> pool;
		move(first, last, std::back_inserter(pool));

		size_t cur_pos{ 0u }, count{ 0u }, size{ static_cast<size_t>(std::distance(first, last)) };
		vector<bool> isKilled(size, false);

		while (true) {
			*first++ = move(pool[cur_pos]);
			isKilled[cur_pos] = true;

			if (++count == size) {
				break;
			}
			// Here we caclulate position of next dead man skipping already killed
			// and cheking boundaries
			for (size_t cur_shift = 0; cur_shift < step_size; ) {
				cur_pos = (++cur_pos == size) ? 0 : cur_pos;
				if (!isKilled[cur_pos]) {
					++cur_shift;
				}
			}
		}
	}

  // Task 2. Split string without copy

  template <typename String>
  using Group = vector<String>;

  template <typename String>
  using Char = typename String::value_type;

  template <typename String>
  vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    map<set<Char<String>>, Group<String>> groupMap;
    set<Char<String>> curKey;

    for (auto& curString : strings) {
      std::for_each(begin(curString), end(curString), [&curKey](const Char<String>& symbol) {
        curKey.insert(symbol);
      });
      auto keyIter = groupMap.find(curKey);
      if (keyIter != groupMap.end()) {
        keyIter->second.push_back(move(curString));
      }
      else {
        groupMap[move(curKey)].push_back(move(curString));
      }
      curKey.clear();
    }
    vector<Group<String>> res;	res.reserve(groupMap.size());
    for (auto& c : groupMap) {
      res.push_back(move(c.second));
    }
    return move(res);
  }

  // Task 3. Split text into sentences without copy
  // Hint: Main idea here to get to TIME LIMIT: allocate memory for vector & move 
  // element by element, not whole vector at once and call clear after move. Than
  // capacity will not change and there will be no reallocations.

  template <typename Token>
  using Sentence = vector<Token>;

  template <typename Token>
  vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
	  vector<Sentence<Token>>	result;
	  if (tokens.empty()) {
		  return result;
	  }
	  bool isPrevEnd = tokens[0].IsEndSentencePunctuation();

	  Sentence<Token> sentence;
	  sentence.reserve(1'000'000);
	  sentence.push_back(move(tokens[0]));

	  for (auto tokenIter = make_move_iterator(next(begin(tokens))); tokenIter != make_move_iterator(end(tokens)); ++tokenIter) {
		  bool isCurEnd = tokenIter->IsEndSentencePunctuation();

		  if (isPrevEnd && !isCurEnd) {
			  // Create new sentence and move all elements from current sentence
			  result.push_back({});
			  result.back().reserve(sentence.size());

			  for (auto& word : sentence) {
				  result.back().push_back(move(word));
			  }

			  sentence.clear();
			  sentence.push_back(*tokenIter);
		  }
		  else {
			  sentence.push_back(*tokenIter);
		  }
		  isPrevEnd = isCurEnd;
	  }

	  if (!sentence.empty()) {
		  result.push_back(move(sentence));
	  }
	  return result;
  }

  // Task 4. Simple vector with move push_back

  template <typename T>
  class MoveSimpleVector {
  public:
	  MoveSimpleVector() : body_(nullptr), capacity_(0u), size_(0u) {}
	  explicit MoveSimpleVector(size_t size) : body_(new T[size]), capacity_(size), size_(size) {}

	  ~MoveSimpleVector() {
		  delete[] body_;
	  }

	  T& operator[](size_t index) {
		  return body_[index];
	  }

	  T* begin() {
		  return body_;
	  }

	  T* const begin() const {
		  return body_;
	  }

	  T* end() {
		  return body_ + size_;
	  }

	  T* const end() const {
		  return body_ + size_;
	  }

	  size_t Size() const {
		  return size_;
	  }
	  size_t Capacity() const {
		  return capacity_;
	  }

	  void PushBack(T value) {
		  if (capacity_ == 0) {
			  capacity_ = size_ = 1;
			  body_ = new T[size_];
			  *body_ = std::move(value);

			  return;
		  }


		  if (size_ == capacity_)
		  {
			  T* tmp = new T[2u * capacity_];
			  move(begin(), end(), tmp);
			  delete[] body_;

			  body_ = tmp;
			  capacity_ *= 2u;
		  }


		  body_[size_++] = std::move(value);
	  }

  private:
	  T* body_;
	  size_t capacity_;
	  size_t size_;
  };

  // Task 5. 3-way merge sort via move semantics

  template <typename RandomIt>
  void MergeSort(RandomIt range_begin, RandomIt range_end) {
	  size_t length = std::distance(range_begin, range_end);
	  if (length < 2) {
		  return;
	  }

	  vector<typename RandomIt::value_type> res{
		  make_move_iterator(range_begin),
		  make_move_iterator(range_end)
	  };

	  auto iterEndFirst{ res.begin() + length / 3 };
	  auto iterEndSecond{ res.begin() + 2 * length / 3 };

	  MergeSort(res.begin(), iterEndFirst);
	  MergeSort(iterEndFirst, iterEndSecond);
	  MergeSort(iterEndSecond, res.end());

	  vector<typename RandomIt::value_type> tmp;
	  tmp.reserve(std::distance(res.begin(), iterEndFirst));

	  merge(
		  make_move_iterator(res.begin()), make_move_iterator(iterEndFirst),
		  make_move_iterator(iterEndFirst), make_move_iterator(iterEndSecond),
		  back_inserter(tmp)
	  );

	  merge(
		  make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
		  make_move_iterator(iterEndSecond), make_move_iterator(res.end()),
		  range_begin
	  );
  }

  // Task 6. Sum of matrix via async

  template<typename ContainerOfVectors>
  int64_t CalculateSumSingleThread(ContainerOfVectors& result) {
    int64_t subSum{ 0 };
    for (auto& row : result) {
      subSum += std::accumulate(row.begin(), row.end(), 0);
    }

    return subSum;
  }

  int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    int64_t sum{ 0 };

    std::vector<std::future<int64_t>> futures;
    size_t pageSize{ 2250 };
    
    for (auto page : week_1::Paginate(matrix, pageSize)) {
      futures.push_back(
        std::async([page] { return CalculateSumSingleThread(page); })
      );
    }
    for (auto& f : futures) {
      sum += f.get();
    }

    return sum;
  }



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
      d_heap.insert({ 0, d_curId });
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


}