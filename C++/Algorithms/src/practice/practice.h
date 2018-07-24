#pragma once

#include<algorithm>
#include<vector>
#include<string>


namespace practice
{
	using namespace std;

#pragma region Dynamic Programming


	// Stepic: https://stepik.org/lesson/13257/step/5?unit=3442
	int getLengthOfLongestDivisibleSubsequence(const vector<int> &numbers);

	// Codefights: https://app.codesignal.com/interview-practice/task/oJXTWuwEZiC6FTw3A
	// Approach to solution: on the web site
	int climbingStairs(int stairs_number);


	// Codefights: https://app.codesignal.com/interview-practice/task/XBWN6DYRqPrKdMZs8
	// Approach to solution: on the web site
	int houseRobber(std::vector<int> nums);

	// Codefights: https://app.codesignal.com/interview-practice/task/cHYqbQ9DiWmejAdeG
	// Approach to solution: on the web site
	std::vector<std::string> composeRanges(std::vector<int> nums);

	// Codefigths: https://app.codesignal.com/interview-practice/task/mkobsYSSQo3JpvYNN
	// Approach to solution: on the web site
	int maximalSquare(std::vector<std::vector<char>> matrix);


#pragma endregion

}