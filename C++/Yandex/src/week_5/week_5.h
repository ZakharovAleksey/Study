#ifndef WEEK_5_H

#pragma once

#include <iostream>
#include<string>

using namespace std;

#pragma region Наследование своими руками

class Animal {
public:
	Animal(const string & name);
	const string Name;
};

class Dog : public Animal {
public:
	Dog(const string & name);

	void Bark();
};

#pragma endregion

#pragma region Отправка уведомлений

#include <iostream>
#include <string>

using namespace std;

void SendSms(const string& number, const string& message);

void SendEmail(const string& email, const string& message);

class INotifier
{
public:
	virtual void Notify(const string & message) = 0;
};

class SmsNotifier : public INotifier
{
public:
	SmsNotifier(const string & phone_number);
	void Notify(const string & message) override;

private:
	const string phone_number_;
};

class EmailNotifier : public INotifier
{
public:
	EmailNotifier(const string & email_number);
	void Notify(const string & message) override;

private:
	const string email_number_;
};

void Notify(INotifier& notifier, const string& message);

#pragma endregion

#pragma region Набор фигур

#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
#include<memory>
#include<iomanip>

using namespace std;

class Figure
{
public:

	Figure(const string & name) : name_(name) {}

	virtual string Name() { return name_; }
	virtual double Perimeter() = 0;
	virtual double Area() = 0;

	const string name_;
};

class Triangle : public Figure
{
public:
	Triangle(int a, int b, int c) : Figure("TRIANGLE"), a_(a), b_(b), c_(c) {}

	double Perimeter() override;
	double Area() override;

private:
	int a_, b_, c_;
};

class Rect : public Figure
{
public:
	Rect(int width, int height) : Figure("RECT"), width_(width), height_(height) {}

	double Perimeter() override;
	double Area() override;

private:
	int width_, height_;
};

class Circle : public Figure
{
public:
	Circle(int r) : Figure("CIRCLE"), r_(r) {}

	double Perimeter() override;
	double Area() override;

private:
	int r_;
};

shared_ptr<Figure> CreateFigure(istringstream & is);

#pragma endregion

#pragma region Грамотно рефакторим код

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Human
{
public:
	Human(const string & name, const string & profession) :
		Name(name), Profession(profession) { }
	virtual ~Human() {}

	virtual void Walk(const string & destination) const;

	const string Name;
	const string Profession;

protected:
	// Additional method for output overloading
	void TmpStringOutput(string first_part, const string & second_part = "") const;
};

class Student : public Human {
public:

	Student(const string & name, const string & favouriteSong) :
		Human(name, "Student"), FavouriteSong(favouriteSong) { }

	void Learn() const;
	void SingSong() const;

	void Walk(const string & destination) const override;

private:
	const string FavouriteSong;
};

class Teacher : public Human {
public:

	Teacher(const string & name, const string & subject) :
		Human(name, "Teacher"), Subject(subject) { }

	void Teach() const;

private:
	const string Subject;
};

class Policeman : public Human {
public:
	Policeman(const string & name) : Human(name, "Policeman") { }

	void Check(const Human & h) const;
};

void VisitPlaces(const Human & h, const vector<string> & places);

#pragma endregion

#endif // !WEEK_5_H
