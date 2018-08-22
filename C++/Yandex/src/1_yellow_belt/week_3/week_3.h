#ifndef HEADER_WEEK_3_H
#define HEADER_WEEK_3_H

#pragma once

#pragma region –еализовать функции, объ€вленные в заголовочном файле

#include <string>
#include <vector>

using namespace std;

int Sum(int x, int y);
string Reverse(string s);
void Sort(vector<int>& nums);


#pragma endregion

#pragma region –еализовать класс, объ€вленный в заголовочном файле

#include <string>

using namespace std;

class PhoneNumber {
public:
	explicit PhoneNumber(const string &international_number);

	string GetCountryCode() const;
	string GetCityCode() const;
	string GetLocalNumber() const;
	string GetInternationalNumber() const;

private:
	string country_code_;
	string city_code_;
	string local_number_;
};


#pragma endregion

#endif // !HEADER_WEEK_3_H
