#pragma once

#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<iterator>

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



}