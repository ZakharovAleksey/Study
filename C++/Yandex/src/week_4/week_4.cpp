#include "week_4.h"

#include<iostream>
#include <numeric>
#include<deque>
#include<string>

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

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border)
{
	// Check if set is empty
	if (numbers.empty())
		return end(numbers);
	// Check if border is in the set
	auto pos = numbers.find(border);
	if (pos != end(numbers))
		return pos;
	else
	{
		// If border is less than min element in set
		if (border <= *begin(numbers))
			return begin(numbers);
		// If border is higher than max element in set
		else if (border >= *prev(end(numbers)))
			return prev(end(numbers));
		// If border is n range (min, max)
		else
		{
			// Position of the next to border element 
			auto right_pos = lower_bound(begin(numbers), end(numbers), border);
			// Position of the previous to the border element
			auto left_pos  = prev(right_pos);

			// Return the closest elemet to border 
			if (abs(border - *left_pos) <= abs(*right_pos - border))
				return left_pos;
			else
				return right_pos;
		}
	}
}

bool CompareOperations(const string & prev, const string cur)
{
	const vector<string> high = { "*", "/" };
	const vector<string> low =  { "+", "-" };

	return (find(begin(high), end(high), cur) != end(high) &&
		find(begin(low), end(low), prev) != end(low)) ? true : false;
}

void ConstructEquationBySymbols()
{
	int x; cin >> x;
	size_t n = 0; cin >> n;
	deque<string> eq = {to_string(x)};
	string line; getline(cin, line);
	string prev_operation = "*";
	string next_operation;

	for (size_t i = 0; i < n; ++i)
	{
		getline(cin, line);
		size_t space_id = line.find(' ');
		next_operation = line.substr(0, space_id);
		if (CompareOperations(prev_operation, next_operation))
		{
			eq.push_front("(");
			eq.push_back(")");
		}
		eq.push_back(" " + next_operation + " ");
		eq.push_back(line.substr(space_id + 1, line.length()));
		prev_operation = next_operation;
	}

	copy(begin(eq), end(eq), ostream_iterator<string>(cout, ""));
	cout << endl;
}

namespace t
{

	void Person::ChangeFirstName(int year, const string& first_name)
	{
		f_name_[year] = first_name;
	}

	void Person::ChangeLastName(int year, const string& last_name)
	{
		l_name_[year] = last_name;
	}

	string Person::GetFullName(int year)
	{
		auto lb_first_name = f_name_.upper_bound(year);
		auto lb_last_name = l_name_.upper_bound(year);

		if (lb_first_name == begin(f_name_) && lb_last_name == begin(l_name_))
			return "Incognito";
		else if (lb_first_name == begin(f_name_) && lb_last_name != begin(l_name_))
			return prev(lb_last_name)->second + " with unknown first name";
		else if (lb_first_name != begin(f_name_) && lb_last_name == begin(l_name_))
			return prev(lb_first_name)->second + " with unknown last name";
		else
			return prev(lb_first_name)->second + " " + prev(lb_last_name)->second;
	}
}