#include<algorithm>
#include <iterator> 
#include<vector>
#include<set>
#include<map>
#include<cstring>

using namespace std;

#pragma region task 1

void PrintVectorPart(const vector<int>& numbers);

#pragma endregion

#pragma region task 2

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border)
{
	auto pos = find_if(begin(elements), end(elements),
		[border](const T & el)
	{
		return el > border;
	});

	return{ pos, end(elements) };
}

#pragma endregion

#pragma region task 3

vector<string> SplitIntoWords(const string& s);

#pragma endregion

#pragma region task 4

template <typename T>
void RemoveDuplicates(vector<T>& elements)
{
	sort(begin(elements), end(elements));
	auto last = unique(begin(elements), end(elements));
	elements.erase(last, elements.end());
}

#pragma endregion

#pragma region task 5

void LexicographicallyRearrangement();

#pragma endregion

#pragma region task 6

enum class Gender {
	FEMALE,
	MALE
};

struct Person {
	int age;  // возраст
	Gender gender;  // пол
	bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
	if (range_begin == range_end) {
		return 0;
	}
	vector<typename InputIt::value_type> range_copy(range_begin, range_end);
	auto middle = begin(range_copy) + range_copy.size() / 2;
	nth_element(
		begin(range_copy), middle, end(range_copy),
		[](const Person& lhs, const Person& rhs) {
		return lhs.age < rhs.age;
	}
	);
	return middle->age;
}

void PrintStats(vector<Person> persons);

#pragma endregion

#pragma region task 7

// Additional my Merge function implementation
template <typename RandomIt>
void Merge(RandomIt range_begin, RandomIt range_med, RandomIt range_end)
{
	auto first_iter = range_begin;
	auto second_iter = range_med;

	vector<typename RandomIt::value_type> tmp(range_end - range_begin);
	auto cur_iter = tmp.begin();

	while (cur_iter != end(tmp))
	{
		// First half is empty: add all second half values to tmp array
		if (first_iter == range_med)
		{
			while (second_iter != range_end)
				*(cur_iter++) = *(second_iter++);
			break;
		}

		// Second half is empty: add all first half values to tmp array
		if (second_iter == range_end)
		{
			while (first_iter != range_med)
				*(cur_iter++) = *(first_iter++);
			break;
		}

		if (*first_iter < *second_iter)
			*cur_iter = *(first_iter++);
		else
			*cur_iter = *(second_iter++);
		++cur_iter;
	}

	for (auto it = range_begin; it != range_end; ++it)
		*it = tmp[it - range_begin];
}

template <typename RandomIt>
void MergeSortDiv2(RandomIt range_begin, RandomIt range_end)
{
	size_t dist = range_end - range_begin;
	if (dist < 2)
		return;

	vector<typename RandomIt::value_type> tmp(range_begin, range_end);

	MergeSortDiv2(begin(tmp), begin(tmp) + dist / 2);
	MergeSortDiv2(begin(tmp) + dist /2 , end(tmp));

	merge(begin(tmp), begin(tmp) + dist / 2, begin(tmp) + dist/ 2, end(tmp), range_begin);

	/* !!! Additional implementation via inlace merge !!!
	MergeSort(range_begin, range_begin + dist / 2);
	MergeSort(range_begin + dist / 2, range_end);

	inplace_merge(range_begin, range_begin + dist / 2, range_end);
	*/
}

template <typename RandomIt>
void MergeSortDiv3(RandomIt range_begin, RandomIt range_end)
{
	size_t dist = range_end - range_begin;
	if (dist < 2)
		return;
	vector<typename RandomIt::value_type> tmp(range_begin, range_end);

	MergeSortDiv2(begin(tmp), begin(tmp) + dist / 3);
	MergeSortDiv2(begin(tmp) + dist / 3, begin(tmp) + 2 * dist / 3);
	MergeSortDiv2(begin(tmp) + 2 * dist / 3, end(tmp));

	vector<typename RandomIt::value_type> lol;
	merge(begin(tmp), begin(tmp) + dist / 3, begin(tmp) + dist / 3, begin(tmp) + 2 * dist / 3, back_inserter(lol));
	merge(begin(lol), end(lol), begin(tmp) + 2 * dist / 3, end(tmp), range_begin);
}

#pragma endregion

#pragma region task 8

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border);

#pragma endregion

#pragma region task 9

namespace t
{

	class Person {
	public:
		// добавить факт изменения имени на first_name в год year
		void ChangeFirstName(int year, const string& first_name);
		// добавить факт изменения фамилии на last_name в год year
		void ChangeLastName(int year, const string& last_name);
		// получить имя и фамилию по состоянию на конец года year с помощью двоичного поиска
		string GetFullName(int year);
	private:
		map<int, string> f_name_;
		map<int, string> l_name_;
	};

}
#pragma endregion

#pragma region task 10
	
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix)
{
	return {
		lower_bound(range_begin, range_end, string(1, prefix)),
		lower_bound(range_begin, range_end, string(1, static_cast<char>(prefix + 1))) };
}

template <typename RandomIt> 
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, const std::string& prefix)
{
	size_t len = prefix.length();
	return std::equal_range(range_begin, range_end, prefix,
		[&prefix](const std::string& left, const std::string& right)
	{
		return left.compare(0, len, right.substr(0, len) ) < 0;
	});
}

#pragma endregion

#pragma region task 11

void ConstructEquationBySymbols();

#pragma endregion
