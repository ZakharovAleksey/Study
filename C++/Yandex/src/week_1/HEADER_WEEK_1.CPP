#include "header_week_1.h"

namespace w1_t2
{
	const TasksInfo & w1_t2::TeamTasks::GetPersonTasksInfo(const string & person) const
	{
		return body_.at(person);
	}

	void w1_t2::TeamTasks::AddNewTask(const string & person)
	{
		if (body_.count(person) == 0)
		{
			TasksInfo tmp =
			{
				{ TaskStatus::NEW, 1 },
				{ TaskStatus::IN_PROGRESS, 0 },
				{ TaskStatus::TESTING, 0 },
				{ TaskStatus::DONE, 0 },
			};

			body_.insert(std::make_pair(person, tmp));

		}
		else
			++body_.at(person).at(TaskStatus::NEW);
	}

	tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(const string & person, int task_count)
	{		
		std::vector<int> non_update_data;
		for (const auto & i : body_.at(person))
			non_update_data.push_back(i.second);
		
		std::vector<int> update_data(4, 0);
		std::vector<int> new_data = non_update_data;

		int left_tasks = task_count;
		int task_id = 0;
		
		while (left_tasks != 0)
		{
			if (non_update_data[task_id] == 0)
			{
				++task_id;
				continue;
			}
			if (non_update_data[task_id] > 0)
			{
				--non_update_data[task_id];
				--new_data[task_id];
				int update_id = (task_id + 1 == 5) ? task_id : task_id + 1;
				++new_data[update_id];
				++update_data[update_id];
				--left_tasks;
			}
		}

		for (int i = 0; i < 4; ++i)
			body_.at(person).at(TaskStatus(i)) = new_data[i];

		TasksInfo update_tuple;
		for (int i = 0; i < 4; ++i)
		{
			if (update_data[i] == 0)
				continue;
			else
				update_tuple.insert(std::make_pair(TaskStatus(i), update_data[i]));
		}

		TasksInfo non_update_tuple;
		for (int i = 0; i < 4; ++i)
		{
			if (non_update_data[i] == 0)
				continue;
			else
				non_update_tuple.insert(std::make_pair(TaskStatus(i), non_update_data[i]));
		}

		return tie(update_tuple, non_update_tuple);
	}
}