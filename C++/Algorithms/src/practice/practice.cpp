#include "practice.h"

#include<iostream>

namespace practice
{
	int practice::getLengthOfLongestDivisibleSubsequence(const vector<int>& numbers)
	{
		vector<int> divis(numbers.size(), 1);
		for (size_t i = 1; i < numbers.size(); ++i)
		{
			vector<int> tmp;
			for (size_t j = 0; j < i; ++j)
			{
				if (numbers[i] % numbers[j] == 0)
					tmp.push_back(divis[j]);
			}
			if (!tmp.empty())
				divis[i] = *max_element(begin(tmp), end(tmp)) + 1;
		}

		return *max_element(begin(divis), end(divis));
	}

	int climbingStairs(int stairs_number)
	{
		if (stairs_number == 1)
			return 1;
		else if (stairs_number == 2)
			return 2;

		vector<int> v(stairs_number, 1);
		v[1] = 2;

		for (size_t i = 2; i < v.size(); ++i)
			v[i] = v[i - 1] + v[i - 2];

		return  v[stairs_number - 1];
	}

	int houseRobber(std::vector<int> nums)
	{
		if (nums.empty())
			return 0;
		if (nums.size() == 1)
			return nums[0];
		if (nums.size() == 2)
			return max(nums[0], nums[1]);

		vector<int> sums(nums);
		for (size_t i = 2; i < nums.size(); ++i)
		{
			int max = *max_element(begin(sums), begin(sums) + i - 1);
			sums[i] += max;
		}

		return max(*prev(end(sums)), *prev(prev(end(sums))));
	}

	std::vector<std::string> composeRanges(std::vector<int> nums)
	{
		if (nums.empty())
			return vector<string>();
		if (nums.size() == 1)
			return vector<string>(1, to_string(nums[0]));

		vector<string> res;
		size_t left_id = 0, right_id = 0;
		for (size_t i = 1; i < nums.size(); ++i)
		{
			if (nums[i] == nums[i - 1] + 1)
				++right_id;
			else
			{
				if (left_id == right_id)
					res.push_back(to_string(nums[left_id]));
				else
					res.push_back(to_string(nums[left_id]) + "->" + to_string(nums[right_id]));
				left_id = right_id = i;
			}
		}
		if (left_id == right_id)
			res.push_back(to_string(nums[left_id]));
		else
			res.push_back(to_string(nums[left_id]) + "->" + to_string(nums[right_id]));

		return res;
	}

	int maximalSquare(std::vector<std::vector<char>> matrix)
	{
		if (matrix.empty())
			return 0;

		else if (matrix.size() == 1)
			return (find(begin(matrix[0]), end(matrix[0]), '1') == end(matrix[0])) ? 0 : 1;

		size_t Y = matrix.size();
		size_t X = matrix[0].size();

		int** M = new int*[Y];
		for (size_t i = 0; i < Y; ++i)
			M[i] = new int[X];

		int max = 0;
		for (size_t x = 0; x < X; ++x)
		{
			M[0][x] = matrix[0][x] - '0';
			if (M[0][x] == 1)
				max = 1;
		}

		for (size_t y = 0; y < Y; ++y)
		{
			M[y][0] = matrix[y][0] - '0';
			if (M[y][0] == 1)
				max = 1;
		}

		for (size_t y = 1; y < Y; ++y)
			for (size_t x = 1; x < X; ++x)
			{
				if (matrix[y][x] == '0')
					M[y][x] = 0;
				else
				{
					vector<int> v = { M[y - 1][x - 1], M[y][x - 1], M[y - 1][x] };
					M[y][x] = *min_element(begin(v), end(v)) + 1;
				}
				if (M[y][x] > max)
					max = M[y][x];
			}

		return max * max;
	}


}
