#include "database.h"

Database::Database() {}

void Database::Add(const Date & date, const string & event)
{
	if (( body_.find(date) == end(body_) || // There is no such date in map 
		( body_.find(date) != end(body_) && body_[date].find(event) == end(body_[date])))  // Date is in map but the event is epcent
		)
	{
		// Add pair<date, event> to database
		body_[date].insert(event);
		// Add order of previously added pair<date, event>
		order_.insert(make_pair(date, body_.find(date)->second.find(event)));
	}
}

ostream & Database::Print(ostream & os)
{
	// TODO: Via copy algorithm
	for (const auto & pair : order_)
		os << pair.first << " " << *pair.second << endl;

	return os;
}

size_t Database::RemoveIf(Func predicate)
{
	// Via remove_if algorithm
	size_t num = 0;
	for (auto it = begin(order_); it != end(order_); )
	{
		if (predicate(it->first, *it->second))
		{
			it = order_.erase(it);
			++num;
		}
		else
			++it;
	}
	
	for (auto iter : body_)
	{
		for (auto it = begin(iter.second); it != end(iter.second); )
		{
			if (predicate(iter.first, *it))
				it = iter.second.erase(it);
			else
				++it;
		}
	}

	for (auto iter = begin(body_); iter != end(body_);)
	{
		if (iter->second.empty())
			iter = body_.erase(iter);
		else
			++iter;
	}
	return num;
}

const multimap<Date, string> Database::FindIf(Func predicate) const
{
	multimap<Date, string> result;

	/*copy_if(begin(order_), end(order_), back_inserter(result),
	[predicate](const pair<const Date, const set<string>::const_iterator> p)
	{
	return predicate(p.first, *p.second);
	});*/

	for (auto & it : order_)
		if (predicate(it.first, *it.second))
			result.insert(make_pair(it.first, *it.second));

	return result;
}

const string Database::Last(const Date & date)
{
	auto upper_bound = order_.upper_bound(date);

	// If data is lower than all dates in database
	if (upper_bound == begin(order_))
		return "No entries";
	// Previous last added data and event
	upper_bound = prev(upper_bound);

	// Write output data
	ostringstream os;
	os << upper_bound->first << " " << *(upper_bound->second);
	return os.str();
}
