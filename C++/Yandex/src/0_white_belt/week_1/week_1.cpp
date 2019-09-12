#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Task 1: Sum two element
void addTwoElements()
{
  int a, b;
  cin >> a >> b;
  cout << a + b << endl;
}

// Task 2: Minimal string
void minimalString()
{
  string a, b, c;
  cin >> a >> b >> c;
  set<string> s = { a, b, c };
  cout << *begin(s) << endl;
}

// Task 3: Square equation solver
void SquareEquationSolver()
{
  double a{ 0. }, b{ 0. }, c{ 0. };
  cin >> a >> b >> c;
  double x1{ 0. }, x2{ 0. };
  double D = b * b - 4. * a * c;

  if (a == 0 && b == 0)
  {
    cout << endl;
  } else if (a == 0 && b != 0)
  {
    cout << -c / b << endl;
  } else if (D >= 0)
  {
    x1 = (-b + sqrt(D)) / (2. * a);
    x2 = (-b - sqrt(D)) / (2. * a);
    if (x1 != x2)
    {
      cout << x1 << " " << x2 << endl;
    } else
    {
      cout << x1 << endl;
    }
  } else
  {
    cout << endl;
  }
}

// Task 4: division of two numbers
void Division()
{
  int a, b;
  cin >> a >> b;
  if (b == 0)
  {
    cout << "Impossible" << endl;
  } else
  {
    cout << a / b << endl;
  }
}

// Task 5: Final price
void GetFinalPreice()
{
  double n, a, b, x, y;
  cin >> n >> a >> b >> x >> y;

  if (n <= a)
  {
    cout << n << endl;
  } else if (n > a && n <= b)
  {
    cout << (1. - x / 100.) * n << endl;
  } else
  {
    cout << (1. - y / 100.) * n << endl;
  }
}

// Task 6: Print all even numbers in range [a; b]
void PrintAllEvenNumbers()
{
  int a{ 0 }, b{ 0 };
  cin >> a >> b;
  int start = a;
  if (a % 2 != 0)
  {
    ++start;
  }

  while (start <= b)
  {
    cout << start << " ";
    start += 2;
  }
}

// Task 7: Find index of second repetition of 'f' character
void FindSecondF()
{
  string str;
  cin >> str;
  int index{ -2 };
  size_t cur_rep_numb{ 0 };

  for (size_t i = 0; i < str.size(); ++i)
  {
    if (str[i] == 'f' && cur_rep_numb == 0)
    {
      ++cur_rep_numb;
      index = -1;
    } else if (str[i] == 'f' && cur_rep_numb == 1)
    {
      index = static_cast<int>(i);
      break;
    }
  }

  cout << index << endl;
}

// Task 8: Greatest common divisor
void Divisor()
{
  int a{ 1 }, b{ 1 };
  cin >> a >> b;

  while (a > 0 && b > 0)
  {
    if (a > b)
    {
      a %= b;
    } else
    {
      b %= a;
    }
  }

  cout << a + b << endl;
}

// Task 9: Binary view of number
void ToBinary()
{
  int number{ 0 };
  cin >> number;
  vector<int> v;
  while (number != 1)
  {
    v.push_back(number % 2);
    number /= 2;
  }
  v.push_back(1);
  copy(rbegin(v), rend(v), ostream_iterator<int>(cout, ""));
  cout << endl;
}
