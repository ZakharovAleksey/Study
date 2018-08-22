#include "node.h"

bool EmptyNode::Evaluate(const Date & Date, const string & event)
{
	return true;
}

DateComparisonNode::DateComparisonNode(const Comparison & cmp, const Date & date) : cmp_(cmp), cmp_date_(date) {}

bool DateComparisonNode::Evaluate(const Date & Date, const string & event)
{
	if (cmp_ == Comparison::Equal)
		return Date == cmp_date_;
	else if (cmp_ == Comparison::Greater)
		return Date > cmp_date_;
	else if (cmp_ == Comparison::GreaterOrEqual)
		return Date >= cmp_date_;
	else if (cmp_ == Comparison::Less)
		return Date < cmp_date_;
	else if (cmp_ == Comparison::LessOrEqual)
		return Date <= cmp_date_;
	else if (cmp_ == Comparison::NotEqual)
		return Date != cmp_date_;
	else
		throw logic_error("Wrong input comparison operator in Evaluate (Date) function.");
}

EventComparisonNode::EventComparisonNode(const Comparison & cmp, const string & value) : cmp_(cmp), cmp_event_(value) {}

bool EventComparisonNode::Evaluate(const Date & Date, const string & event)
{
	if (cmp_ == Comparison::Equal)
		return event == cmp_event_;
	else if (cmp_ == Comparison::Greater)
		return event > cmp_event_;
	else if (cmp_ == Comparison::GreaterOrEqual)
		return event >= cmp_event_;
	else if (cmp_ == Comparison::Less)
		return event < cmp_event_;
	else if (cmp_ == Comparison::LessOrEqual)
		return event <= cmp_event_;
	else if (cmp_ == Comparison::NotEqual)
		return event != cmp_event_;
	else
		throw logic_error("Wrong input comparison operator in Evaluate (Event) function.");
}

LogicalOperationNode::LogicalOperationNode(
	const LogicalOperation & logical_operation, 
	const shared_ptr<Node> left, 
	const shared_ptr<Node> right) : lo_(logical_operation), left_(left), right_(right) {}

bool LogicalOperationNode::Evaluate(const Date & Date, const string & event)
{
	if (lo_ == LogicalOperation::And)
		return left_->Evaluate(Date, event) && right_->Evaluate(Date, event);
	else if (lo_ == LogicalOperation::Or)
		return left_->Evaluate(Date, event) || right_->Evaluate(Date, event);
	else
		throw logic_error("Unknown logic expression. ");
}