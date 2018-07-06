#include "week_5.h"

Animal::Animal(const string & name) : Name(name) {}

Dog::Dog(const string & name) : Animal(name) {}

void Dog::Bark()
{
	cout << Name << " barks: woof!" << endl;
}

void SendSms(const string & number, const string & message)
{
	cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string & email, const string & message)
{
	cout << "Send '" << message << "' to e-mail " << email << endl;
}

SmsNotifier::SmsNotifier(const string & phone_number) : phone_number_(phone_number) {}

void SmsNotifier::Notify(const string & message)
{
	SendSms(phone_number_, message);
}

EmailNotifier::EmailNotifier(const string & email_number) : email_number_(email_number) {}

void EmailNotifier::Notify(const string & message)
{
	SendEmail(email_number_, message);
}

void Notify(INotifier & notifier, const string & message)
{
	notifier.Notify(message);
}

double Triangle::Perimeter()
{
	return a_ + b_ + c_;
}

double Triangle::Area()
{
	double p = Perimeter() / 2.f;
	return sqrt(p * (p - a_) * (p - b_) * (p - c_));
}

double Rect::Perimeter()
{
	return (width_ + height_) * 2.f;
}

double Rect::Area()
{
	return width_ * height_;
}

double Circle::Perimeter()
{
	return 2. * 3.14 * r_;
}

double Circle::Area()
{
	return 3.14 * r_ * r_;
}

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

void Human::Walk(const string & destination) const
{
	cout << Profession << ": " << Name << " walks to: " << destination << endl;
}

void Human::TmpStringOutput(string first_part, const string & second_part) const
{
	cout << Profession << ": " << Name << first_part << second_part << endl;
}

void Student::Learn() const
{
	TmpStringOutput(" learns");
}

void Student::SingSong() const
{
	TmpStringOutput(" sings a song: ", FavouriteSong);
}

void Student::Walk(const string & destination) const
{
	Human::Walk(destination);
	SingSong();
}

void Teacher::Teach() const
{
	TmpStringOutput(" teaches: ", Subject);
}

void Policeman::Check(const Human & h) const
{
	cout << Profession << ": " << Name << " checks " << h.Profession << ". "
		<< h.Profession << "\'s name is: " << h.Name << endl;
}

void VisitPlaces(const Human & h, const vector<string>& places)
{
	for (const auto & p : places)
		h.Walk(p);
}