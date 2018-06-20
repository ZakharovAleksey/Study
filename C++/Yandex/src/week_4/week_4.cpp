#include "week_4.h"

#include<iostream>
#include <numeric>

void PrintVectorPart(const vector<int>& numbers)
{
	auto pos = find_if(begin(numbers), end(numbers), 
		[](const int & val)
	{
		return val < 0;
	});

	while (pos != begin(numbers))
	{
		--pos;
		cout << *pos << " ";
	}
	cout << endl;
}

vector<string> SplitIntoWords(const string & s)
{
	vector<string> res;
	string::const_iterator word_begin = begin(s);
	string::const_iterator word_end = begin(s);

	while (word_end != end(s))
	{
		word_end = find_if(word_begin, end(s), 
			[](const char & symbol)
		{
			return symbol == ' ';
		});

		res.push_back({ word_begin, word_end });
		word_begin = (word_end == end(s)) ? end(s) : word_end + 1;
	}
	return res;
}

void LexicographicallyRearrangement()
{
	size_t n; cin >> n;
	
	vector<int> v(n);
	iota(v.begin(), v.end(), 1);
	sort(rbegin(v), rend(v));

	ostream_iterator<int> out_it(cout, " ");
	do 
	{
		copy(begin(v), end(v), out_it); 
		cout << endl;
	} while (prev_permutation(begin(v), end(v)));
}

void PrintStats(vector<Person> persons)
{
	cout << "Median age = " << ComputeMedianAge(begin(persons), end(persons)) << endl;
	// Females / Males
	auto f_m_pos = partition(begin(persons), end(persons), [](const Person & p)
	{
		return p.gender == Gender::FEMALE;
	}
	);
	cout << "Median age for females = " << ComputeMedianAge(begin(persons), f_m_pos) << endl;
	cout << "Median age for males = " << ComputeMedianAge(f_m_pos, end(persons)) << endl;

	// Females: employed / unemployed
	auto f_emp_pos = partition(begin(persons), f_m_pos, [](const Person & p)
	{
		return p.is_employed;
	}
	);
	cout << "Median age for employed females = " << ComputeMedianAge(begin(persons), f_emp_pos) << endl;
	cout << "Median age for unemployed females = " << ComputeMedianAge(f_emp_pos, f_m_pos) << endl;
	// Males: employed / unemployed
	auto m_emp_pos = partition(f_m_pos, end(persons), [](const Person & p)
	{
		return p.is_employed;
	}
	);
	cout << "Median age for employed males = " << ComputeMedianAge(f_m_pos, m_emp_pos) << endl;
	cout << "Median age for unemployed males = " << ComputeMedianAge(m_emp_pos, end(persons)) << endl;
}
