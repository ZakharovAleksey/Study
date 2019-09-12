#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace WhiteBeltW4
{
  using namespace std;

  struct Image
  {
    double quality;
    double freshness;
    double rating;
  };

  struct Params
  {
    double a;
    double b;
    double c;
  };

  class FunctionPart {
    public:
    FunctionPart(char operation, double value);

    double Apply(double sourceValue) const;
    void Invert();

    private:
    char d_operation;
    double d_value;
  };

  class Function {
    public:
    void AddPart(const char operation, double value);
    double Apply(double value) const;
    void Invert();

    private:
    vector<FunctionPart> d_parts;
  };

  Function MakeWeightFunction(const Params& params, const Image& image);
  double ComputeImageWeight(const Params& params, const Image& image);
  double ComputeQualityByWeight(const Params& params, const Image& image,
                                double weight);

  struct Student
  {
    std::string d_fName;
    std::string d_lName;
    long int d_day;
    long int d_month;
    long int d_year;

    friend istream& operator>>(std::istream& is, Student& st);
    void PrintName() const;
    void PrintDate() const;
  };

  class Rational {
    public:
    Rational() : d_numenator(0), d_denominator(1) {}
    Rational(int numerator, int denominator);

    int Numerator() const;
    int Denominator() const;

    friend istream& operator>>(istream& is, Rational& r);
    friend ostream& operator<<(ostream& os, const Rational& r);

    private:
    int Divisor(int numenator, int denumenator);
    int d_numenator;
    int d_denominator;
  };

  bool operator==(const Rational& left, const Rational& right);
  Rational operator+(const Rational& left, const Rational& right);
  Rational operator-(const Rational& left, const Rational& right);
  Rational operator*(const Rational& left, const Rational& right);
  Rational operator/(const Rational& left, const Rational& right);
  bool operator<(const Rational& left, const Rational& right);

  void EnsureEqual(const string& left, const string& right);
  void Calculator();
}  // namespace WhiteBeltW4
