#ifndef WEEK_5_H

#pragma once

#include <iostream>
#include<string>

using namespace std;

#pragma region Наследование своими руками

class Animal {
public:
	Animal(const string & name) : Name(name) {}
	const string Name;
};


class Dog : public Animal {
public:
	Dog(const string & name) : Animal(name) {}

	void Bark() {
		cout << Name << " barks: woof!" << endl;
	}
};

#pragma endregion

#pragma region Отправка уведомлений

#include <iostream>
#include <string>

using namespace std;

void SendSms(const string& number, const string& message) {
	cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string& email, const string& message) {
	cout << "Send '" << message << "' to e-mail " << email << endl;
}


class INotifier
{
public:
	virtual void Notify(const string & message) = 0;
};

class SmsNotifier : public INotifier
{
public:
	SmsNotifier(const string & phone_number) : phone_number_(phone_number) {}
	void Notify(const string & message) override
	{
		SendSms(phone_number_, message);
	}

private:
	const string phone_number_;
};

class EmailNotifier : public INotifier
{
public:
	EmailNotifier(const string & email_number) : email_number_(email_number) {}
	void Notify(const string & message) override
	{
		SendEmail(email_number_, message);
	}

private:
	const string email_number_;
};

void Notify(INotifier& notifier, const string& message) {
	notifier.Notify(message);
}

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

	double Perimeter() override
	{
		return a_ + b_ + c_;
	}

	double Area() override
	{
		double p = Perimeter() / 2.f;
		return sqrt(p * (p - a_) * (p - b_) * (p - c_));
	}

private:
	int a_, b_, c_;
};

class Rect : public Figure
{
public:
	Rect(int width, int height) : Figure("RECT"), width_(width), height_(height) {}

	double Perimeter() override
	{
		return (width_ + height_) * 2.f;
	}
	double Area() override
	{
		return width_ * height_;
	}

private:
	int width_, height_;
};

class Circle : public Figure
{
public:
	Circle(int r) : Figure("CIRCLE"), r_(r) {}

	double Perimeter() override
	{
		return 2. * 3.14 * r_;
	}
	double Area() override
	{
		return 3.14 * r_ * r_;
	}

private:
	int r_;
};

shared_ptr<Figure> CreateFigure(istringstream & is)
{
	string figure_type; is >> figure_type;

	if (figure_type == "RECT")
	{
		int width, height; is >> width >> height;
		return make_shared<Rect>(width, height);
	}
	else if (figure_type == "TRIANGLE")
	{
		int a, b, c; is >> a >> b >> c;
		return make_shared<Triangle>(a, b, c);
	}
	else if (figure_type == "CIRCLE")
	{
		int r; is >> r;
		return make_shared<Circle>(r);
	}
}

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

	virtual void Walk(const string & destination) const
	{
		cout << Profession << ": " << Name << " walks to: " << destination << endl;
	}

	const string Name;
	const string Profession;

protected:
	// Additional method for output overloading
	void TmpStringOutput(string first_part, const string & second_part = "") const
	{
		cout << Profession << ": " << Name << first_part << second_part << endl;
	}

};

class Student : public Human {
public:

	Student(const string & name, const string & favouriteSong) :
		Human(name, "Student"), FavouriteSong(favouriteSong) { }

	void Learn() const
	{
		TmpStringOutput(" learns");
	}

	void SingSong() const
	{
		TmpStringOutput(" sings a song: ", FavouriteSong);
	}

	void Walk(const string & destination) const override
	{
		Human::Walk(destination);
		SingSong();
	}

private:
	const string FavouriteSong;
};

class Teacher : public Human {
public:

	Teacher(const string & name, const string & subject) :
		Human(name, "Teacher"), Subject(subject) { }

	void Teach() const
	{
		TmpStringOutput(" teaches: ", Subject);
	}

private:
	const string Subject;
};

class Policeman : public Human {
public:
	Policeman(const string & name) : Human(name, "Policeman") { }

	void Check(const Human & h) const
	{
		cout << Profession << ": " << Name << " checks " << h.Profession << ". "
			<< h.Profession << "\'s name is: " << h.Name << endl;
	}
};

void VisitPlaces(const Human & h, const vector<string> & places)
{
	for (const auto & p : places)
		h.Walk(p);
}

#pragma endregion

#endif // !WEEK_5_H
