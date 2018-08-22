#pragma once

#include"date.h"

#include<memory>

enum class LogicalOperation
{
	Or,
	And
};

enum class Comparison
{
	Less,
	Greater,
	GreaterOrEqual,
	LessOrEqual,
	Equal,
	NotEqual
};

class Node
{
public:
	Node() {}
	virtual bool Evaluate(const Date& Date, const string& event) = 0;
};

class EmptyNode : public Node
{
public:
	explicit EmptyNode() {}
	bool Evaluate(const Date& Date, const string& event) override;
};

// Performs comparison based on Date field
class DateComparisonNode : public Node
{
public:
	explicit DateComparisonNode(const Comparison & cmp, const Date & date);
	bool Evaluate(const Date& Date, const string& event) override;

private:
	const Comparison cmp_;
	const Date cmp_date_;
};

// Performs comparison based on Event field
class EventComparisonNode : public Node
{
public:
	EventComparisonNode(const Comparison & cmp, const string& value);
	bool Evaluate(const Date& Date, const string& event) override;

private:
	const Comparison cmp_;
	const string cmp_event_;
};

// Performs comparison based on Logical operator for both: Date and Event fields
class LogicalOperationNode : public Node
{
public:
	LogicalOperationNode(const LogicalOperation & logical_operation, 
		const shared_ptr<Node> left, 
		const shared_ptr<Node> right);

	bool Evaluate(const Date& Date, const string& event) override;

private:
	LogicalOperation lo_;
	shared_ptr<Node> left_;
	shared_ptr<Node> right_;
};