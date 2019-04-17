#pragma once

#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<iterator>

namespace RBWeek5{

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


}