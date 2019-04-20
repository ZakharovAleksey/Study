#include"week_4.h"

#include<algorithm>
#include<iterator>
#include<sstream>

namespace WhiteBeltW4 {

	void WhiteBeltW4::Student::PrintName() const {
		cout << d_fName << " " << d_lName << endl;
	}

	void WhiteBeltW4::Student::PrintDate() const {
		cout << d_day << "." << d_month << "." << d_year << endl;
	}

	WhiteBeltW4::FunctionPart::FunctionPart(char operation, double value) : d_operation(operation), d_value(value) {}

	double WhiteBeltW4::FunctionPart::Apply(double sourceValue) const {
		if (d_operation == '+') {
			return sourceValue + d_value;
		}
		else if (d_operation == '-') {
			return sourceValue - d_value;
		}
		else if (d_operation == '*') {
			return sourceValue * d_value;
		}
		else {
			return sourceValue / d_value;
		}
	}

	void WhiteBeltW4::FunctionPart::Invert() {
		if (d_operation == '+') {
			d_operation = '-';
		}
		else if (d_operation == '-') {
			d_operation = '+';
		}
		else if (d_operation == '*') {
			d_operation = '/';
		}
		else {
			d_operation = '*';
		}
	}

	void WhiteBeltW4::Function::AddPart(const char operation, double value) {
		d_parts.push_back({ operation, value });
	}

	double WhiteBeltW4::Function::Apply(double value) const {
		for (const auto& part : d_parts) {
			value = part.Apply(value);
		}
		return value;
	}

	void WhiteBeltW4::Function::Invert() {
		for (auto & part : d_parts) {
			part.Invert();
		}

		reverse(begin(d_parts), end(d_parts));
	}

	Function MakeWeightFunction(const Params & params, const Image & image) {
		Function function;
		function.AddPart('*', params.a);
		function.AddPart('-', image.freshness * params.b);
		function.AddPart('+', image.rating * params.c);
		return function;
	}

	double ComputeImageWeight(const Params & params, const Image & image) {
		Function function = MakeWeightFunction(params, image);
		return function.Apply(image.quality);
	}

	double ComputeQualityByWeight(const Params & params, const Image & image, double weight) {
		Function function = MakeWeightFunction(params, image);
		function.Invert();
		return function.Apply(weight);
	}


	istream& operator >> (std::istream &is, Student &st) {
		is >> st.d_fName >> st.d_lName >> st.d_day >> st.d_month >> st.d_year;
		return is;
	}

	Rational::Rational(int numerator, int denominator) {
		if (denominator == 0) {
			throw invalid_argument("Invalid argument");
		}
		if (numerator == 0) {
			d_numenator = 0;
			d_denominator = 1;
		}
		else if (numerator < 0 && denominator < 0) {
			d_numenator = -numerator;
			d_denominator = -denominator;
		}
		else if (numerator < 0 || denominator < 0) {
			d_numenator = -abs(numerator);
			d_denominator = abs(denominator);
		}
		else {
			d_numenator = numerator;
			d_denominator = denominator;
		}

		int d = Divisor(abs(d_numenator), abs(d_denominator));
		d_numenator /= d;
		d_denominator /= d;
	}

	int Rational::Numerator() const {
		return d_numenator;
	}

	int Rational::Denominator() const {
		return d_denominator;
	}

	istream& operator >> (istream & is, Rational& r) {
		int num{ 0 }, denum{ 0 };
		is >> num; is.ignore(1); is >> denum;

		if (denum != 0) {
			Rational tmp{ num, denum };
			r.d_numenator = tmp.d_numenator;
			r.d_denominator = tmp.d_denominator;
		}

		return is;
	}
	
	ostream& operator<<(ostream& os, const Rational& r) {
		os << r.Numerator() << '/' << r.Denominator();
		return os;
	}

	bool operator==(const Rational & left, const Rational & right) {
		return left.Numerator() == right.Numerator() && left.Denominator() == right.Denominator();
	}

	Rational operator+(const Rational & left, const Rational & right) {
		return{
			left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator(),
			left.Denominator() * right.Denominator()
		};
	}

	Rational operator-(const Rational & left, const Rational & right) {
		return{
			left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator(),
			left.Denominator() * right.Denominator()
		};
	}

	Rational operator*(const Rational & left, const Rational & right) {
		return{
			left.Numerator() * right.Numerator(),
			left.Denominator() * right.Denominator()
		};
	}

	Rational operator/(const Rational & left, const Rational & right) {
		if (right.Numerator() == 0) {
			throw domain_error("Division by zero");
		}
		return{
			left.Numerator() * right.Denominator(),
			left.Denominator() * right.Numerator()
		};
	}

	bool operator<(const Rational & left, const Rational & right) {
		return left.Numerator() * right.Denominator() < right.Numerator() * left.Denominator();
	}

	void EnsureEqual(const string & left, const string & right) {
		if (left != right) {
			stringstream ss;
			ss << left << " != " << right;
			throw runtime_error(ss.str());
		}
	}

	void Calculator() {
		int a{ 0 }, b{ 0 }, c{ 0 }, d{ 0 };
		char operation{ ' ' };
		cin >> a; cin.ignore(1);
		cin >> b; cin.ignore(1);
		cin >> operation; cin.ignore();
		cin >> c; cin.ignore(1);
		cin >> d;

		try {
			Rational first{ a, b };
			Rational second{ c, d };

			switch (operation)
			{
			case '+':
				cout << first + second;
				break;
			case '-':
				cout << first - second;
				break;
			case '*':
				cout << first * second;
				break;
			case '/':
				cout << first / second;
				break;
			default:

				break;
			};
		}
		catch (exception& e) {
			cout << e.what();
		}

	}

	int Rational::Divisor(int numenator, int denumenator) {
		while (numenator > 0 && denumenator > 0) {
			if (numenator > denumenator) {
				numenator %= denumenator;
			}
			else {
				denumenator %= numenator;
			}
		}
		return denumenator + numenator;
	}


}