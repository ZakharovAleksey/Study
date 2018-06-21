#include"unit_test\unit_test.h"
#include"week_4\week_4.h"
#include<iostream>

using namespace std;
using namespace unit_test;

#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>


int main() 
{
	TestRunner tr;
	
	using namespace t;
	t::Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	return 0;


	return 0;
}