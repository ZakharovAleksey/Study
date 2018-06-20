#include<algorithm>
#include <iterator> 
#include<vector>
#include<set>
#include<cstring>

using namespace std;

#pragma region Часть вектора

void PrintVectorPart(const vector<int>& numbers);

#pragma endregion

#pragma region Часть множества

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

#pragma region Разбиение на слова

vector<string> SplitIntoWords(const string& s);

#pragma endregion

#pragma region Удаление повторов

template <typename T>
void RemoveDuplicates(vector<T>& elements)
{
	sort(begin(elements), end(elements));
	auto last = unique(begin(elements), end(elements));
	elements.erase(last, elements.end());
}

#pragma endregion

#pragma region Перебор перестановок

void LexicographicallyRearrangement();

#pragma endregion

#pragma region Демографические показатели

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

#pragma region Сортировка слиянием

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
