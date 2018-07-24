#include"sorting.h"
#include"../unit_test/unit_test.h"

#include<numeric>

namespace sort
{
	void TestInsertionSort()
	{
		TestSortTemplate(InsertionSort<int>, "InsertionSort");
		TestSortTemplateIter(InsertionSortIter<vector<int>::iterator>, "InsertionSortIter");
	}

	void TestSelectSort()
	{
		TestSortTemplate(SelectSort<int>, "SelectionSort");
	}

	void BubbleSortTest()
	{
		TestSortTemplate(BubbleSort<int>, "BubbleSort");
	}
	
	void HeapSortTest()
	{
		TestSortTemplate(HeapSort<int>, "HeapSort");
	}
	
	void MergeSortTest()
	{
		TestSortTemplateIter(MergeSort<vector<int>::iterator>, "MergeSort");
	}
	
	void QuickSortTest()
	{
		TestSortTemplateIter(QuickSort<vector<int>::iterator>, "QuickSort");
		
		vector<int> v = GenerateGaussVector<int>(100);
		vector<int> sorted_v(v); std::sort(begin(sorted_v), end(sorted_v));

		QuickSortOptimizeIter(begin(v), end(v));
		unit_test::AssertEqual(v, sorted_v, "QuickSortOptimizeIter");

	}
	
	void KthOrderStatisticsTest()
	{
		vector<int> v = GenerateGaussVector<int>(25);
		vector<int> order_stats(begin(v), end(v));
		std::sort(begin(order_stats), end(order_stats));

		for (size_t k = 0; k < v.size(); ++k)
		{
			vector<int> tmp(v);
			int pivot = 0; KthOrderStatistics(v, begin(v), end(v), k, pivot);
			unit_test::AssertEqual(pivot, order_stats[k], "pivot order " + to_string(k));
		}

	}

	void CountingSort(vector<int>& v)
	{
		auto it = find_if(begin(v), end(v), [](const int & el) {return el < 0; });

		if (it != end(v))
			throw runtime_error("Input array for Counting Sort contains negative elements");

		int max = *max_element(begin(v), end(v));

		vector<int> count_v(max + 1);

		for (const auto & i : v) 
			++count_v[i];

		auto cur_it = begin(v);
		for (size_t i = 0; i < count_v.size(); ++i)
		{
			while (count_v[i] > 0)
			{
				*cur_it = i;
				++cur_it; --count_v[i];
			}
		}
	}

	void CountingSortInter(vector<int>::iterator range_begin, vector<int>::iterator range_end)
	{
		auto it = find_if(range_begin, range_end, [](const int & el) {return el < 0; });

		if (it != range_end)
			throw runtime_error("Input array for Counting Sort contains negative elements");

		int max = *max_element(range_begin, range_end);

		vector<int> count_v(max + 1);

		for (auto it = range_begin; it != range_end; ++it)
			++count_v[*it];
		
		auto cur_it = range_begin;
		for (size_t i = 0; i < count_v.size(); ++i)
		{
			while (count_v[i] > 0)
			{
				*cur_it = i;
				++cur_it; --count_v[i];
			}
		}
	}

	void CountingSortTest()
	{
		vector<int> v = GenerateGaussVector<int>(5, 10, 3);
		transform(begin(v), end(v), begin(v), [](const int & el) {return abs(el); });
		vector<int> sorted_v(v); std::sort(begin(sorted_v), end(sorted_v));

		CountingSortInter(begin(v), end(v)); // CountingSort(v);
		unit_test::AssertEqual(sorted_v, v, "CountingSort");
	}

}