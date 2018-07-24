#pragma once

#include<iostream>
#include<fstream>
#include<iterator>

#include<vector>
#include<algorithm>
#include<tuple>
#include <random>
#include <chrono>

// Ostream operator for pair<Key, Value>
template<class U, class V>
std::ostream & operator<<(std::ostream & os, const std::pair<U, V> & p)
{
	os << p.first << " " << p.second << endl;
	return os;
}

// Ostream operator for vector<T>
template<class T>
std::ostream & operator<<(std::ostream & os, const std::vector<T> & v)
{
	std::ostream_iterator<T> out_it(os, " ");
	std::copy(begin(v), end(v), out_it);
	return os;
}

// Comparison operator for vector (Equal) : nesessary for unit_test
template<class T>
bool operator==(const std::vector<T> & left, const std::vector<T> & right)
{
	return std::equal(begin(left), end(left), begin(right));
}

// Comparison operator for vector (Not Equal) : nesessary for unit_test
template<class T>
bool operator!=(const std::vector<T> & left, const std::vector<T> & right)
{
	return !(left == right);
}

namespace sort
{
	using namespace std;

	// Generates vector with Normal distribution ~N(10, 20)
	template<class T>
	vector<T> GenerateGaussVector(size_t size = 100, const int mu = 10, const int sigma = 20)
	{
		srand(1);
		std::default_random_engine generator;
		std::normal_distribution<double> distribution(10.0, 20.0);

		vector<T> v(size, T());
		for (auto & el : v)
			el = distribution(generator);

		return v;
	}
	
	template<class Function>
	void TestSortTemplate(Function sorting_func, const string sorting_type)
	{
		vector<int> v = GenerateGaussVector<int>(100);
		vector<int> sorted_v(v); std::sort(begin(sorted_v), end(sorted_v));

		sorting_func(v);
		unit_test::AssertEqual(v, sorted_v, sorting_type);

		v = GenerateGaussVector<int>(111);
		sorted_v = v; std::sort(begin(sorted_v), end(sorted_v));

		auto start = chrono::high_resolution_clock::now();

		sorting_func(v);

		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		cerr << sorting_type << ": elapsed time " << elapsed.count() << "s." << endl;

		unit_test::AssertEqual(v, sorted_v, sorting_type);

		
	}

	template<class Function>
	void TestSortTemplateIter(Function sorting_func, const string & sorting_type)
	{
		vector<int> v = GenerateGaussVector<int>(100);
		vector<int> sorted_v(v); std::sort(begin(sorted_v), end(sorted_v));

		sorting_func(begin(v), end(v));
		unit_test::AssertEqual(v, sorted_v, sorting_type);

		v = GenerateGaussVector<int>(111);
		sorted_v = v; std::sort(begin(sorted_v), end(sorted_v));

		auto start = chrono::high_resolution_clock::now();

		sorting_func(begin(v), end(v));

		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		cerr << sorting_type << ": elapsed time " << elapsed.count() << "s." << endl;

		unit_test::AssertEqual(v, sorted_v, sorting_type);
	}

#pragma region Insertion Sort

	template<class T>
	void InsertionSort(vector<T> & v)
	{
		for (size_t i = 0; i < v.size() - 1; ++i)
		{
			for (size_t j = i + 1; j > 0 && v[j] < v[j - 1]; --j)
				swap(v[j], v[j - 1]);
		}
	}

	template<typename RandomIt>
	void InsertionSortIter(RandomIt range_begin, RandomIt range_end)
	{
		for (auto ext_it = range_begin; ext_it != prev(range_end); ++ext_it)
		{
			for (auto int_it = next(ext_it); int_it != range_begin && *int_it < *prev(int_it); --int_it)
				swap(*int_it, *prev(int_it));
		}
	}

	void TestInsertionSort();

#pragma endregion

#pragma region Select Sort

	template<class T>
	void SelectSort(vector<T> & v)
	{
		for (auto it = begin(v); it != end(v); ++it)
		{
			auto min_iter = min_element(it, end(v));
			swap(*it, *min_iter);
		}
	}

	void TestSelectSort();

#pragma endregion

#pragma region Bubble Sort

	template<typename T>
	void BubbleSort(vector<T> & v)
	{
		for (auto left_it = begin(v); left_it != prev(end(v)); ++left_it)
		{
			for (auto right_it = next(left_it); right_it != end(v); ++right_it)
				if (*left_it > *right_it)
					swap(*left_it, *right_it);
		}
	}

	void BubbleSortTest();

#pragma endregion

#pragma region Heap Sort

	template<class T>
	void SiftDown(vector<T> & v, int cur_id, int size)
	{
		while (cur_id <= size / 2)
		{
			// Choose max element among two childs v[2i] and v[2i+1]
			int max_child_id = (2 * cur_id < size && v[2 * cur_id + 1] > v[2 * cur_id]) ?
				2 * cur_id + 1 :
				2 * cur_id;

			if (v[cur_id] >= v[max_child_id])
				break;
			// Swap child and parent nodes if v[child] is bigger than v[cur_id]
			swap(v[cur_id], v[max_child_id]);
			cur_id = max_child_id;
		}
	}

	template<class T>
	void HeapSort(vector<T> & v)
	{
		// Make a heap
		for (int cur_id = v.size() / 2; cur_id >= 0; --cur_id)
			SiftDown(v, cur_id, v.size() - 1);

		// Now we have heap with max element in the root
		for (int cur_id = v.size() - 1; cur_id > 0; --cur_id)
		{
			// Swap max element with last
			swap(v[0], v[cur_id]);
			// Sift root element (which we swap)
			SiftDown(v, 0, cur_id - 1);
		}

		// Naive <algorithm> implmentation
		//make_heap(begin(v), end(v));
		//sort_heap(begin(v), end(v));
	}

	void HeapSortTest();

#pragma endregion

#pragma region Merge Sort

	template<typename RandomIt>
	void MergeSort(RandomIt range_begin, RandomIt range_end)
	{
		size_t dist = range_end - range_begin;
		if (dist < 2)
			return;

		vector<typename RandomIt::value_type> tmp(range_begin, range_end);

		// If we could not split array on two EQUAL sub-arrays : increace one
		// half of an array on 1 element
		size_t additional_element = (dist % 2) ? 1 : 0;

		// Requarsive MergeSort function call
		MergeSort(begin(tmp), begin(tmp) + dist / 2 + additional_element);
		MergeSort(begin(tmp) + dist / 2 + additional_element, end(tmp));

		// Merge procedure P.S.: Exists an inplace_merge() method
		merge(
			begin(tmp), begin(tmp) + dist / 2 + additional_element,
			begin(tmp) + dist / 2 + additional_element, end(tmp),
			range_begin);
	}

	void MergeSortTest();

	template<typename RandomIt>
	void KWayMergeSort(RandomIt range_begin, RandomIt range_end, const size_t K)
	{

	}


#pragma endregion

#pragma region Quick Sort

	template<typename RandomIt>
	void QuickSort(RandomIt range_begin, RandomIt range_end)
	{
		if (range_begin == range_end)
			return;

		auto pivot = *next(range_begin, distance(range_begin, range_end) / 2);
		auto left_boundary = partition(range_begin, range_end, [pivot](const RandomIt::value_type & element)
		{
			return element < pivot;
		});
		auto right_boundary = partition(left_boundary, range_end, [pivot](const RandomIt::value_type & element)
		{
			return element <= pivot;
		});

		QuickSort(range_begin, left_boundary);
		QuickSort(right_boundary, range_end);
	}

	template<typename RandomIt>
	void QuickSortOptimizeIter(RandomIt range_begin, RandomIt range_end, const size_t opt_size = 20)
	{
		if (distance(range_begin, range_end) < opt_size)
		{
			InsertionSortIter(range_begin, range_end);
			return;
		}

		RandomIt::value_type pivot = *next(range_begin, distance(range_begin, range_end) / 2);

		auto left_bound = partition(range_begin, range_end, [pivot](const RandomIt::value_type & el)
		{
			return el < pivot;
		});

		auto right_bound = partition(left_bound, range_end, [pivot](const RandomIt::value_type & el)
		{
			return el <= pivot;
		});

		size_t left_len = distance(range_begin, left_bound);
		size_t right_len = distance(right_bound, range_end);

		if (left_len < right_len)
		{
			QuickSort(range_begin, left_bound);
			QuickSort(right_bound, range_end);
		}
		else
		{
			QuickSort(right_bound, range_end);
			QuickSort(range_begin, left_bound);
		}
	}

	void QuickSortTest();

#pragma endregion

#pragma region Kth Order Statistics

	template<typename RandomIt, class T>
	void KthOrderStatistics(vector<T> & v, RandomIt range_begin, RandomIt range_end, const size_t k, T & pivot)
	{
		pivot = *next(range_begin, distance(range_begin, range_end) / 2);

		// Partition procedure
		auto left_bound = partition(range_begin, range_end, [pivot](const RandomIt::value_type & el)
		{
			return el < pivot;
		});

		auto right_bound = partition(left_bound, range_end, [pivot](const RandomIt::value_type & el)
		{
			return el <= pivot;
		});

		// Obtain position [first, last] of pivot after partition procedure is complete
		auto pivot_first_pos = find(range_begin, range_end, pivot);

		size_t pivot_first_id = distance(begin(v), pivot_first_pos);
		size_t numb = count(range_begin, range_end, pivot) - 1;
		size_t pivot_last_id = pivot_first_id + numb;

		if (k >= pivot_first_id && k <= pivot_last_id)
			return;
		else if (k < pivot_first_id)
			KthOrderStatistics(v, range_begin, pivot_first_pos, k, pivot);
		else if (k > pivot_last_id)
			KthOrderStatistics(v, pivot_first_pos + numb, range_end, k, pivot);
	}

	void KthOrderStatisticsTest();

#pragma endregion

#pragma region Counting Sort
	
	void CountingSort(vector<int> & v);
	void CountingSortInter(vector<int>::iterator range_begin, vector<int>::iterator range_end);

	void CountingSortTest();

#pragma endregion

}