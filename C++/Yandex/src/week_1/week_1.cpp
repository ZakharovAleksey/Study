#include "week_1.h"

const TasksInfo & TeamTasks::GetPersonTasksInfo(const string & person) const
{
	return body_.at(person);
}

void TeamTasks::AddNewTask(const string & person)
{
	body_[person][TaskStatus::NEW]++;
}

tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(const string & person, int task_count)
{		
	TasksInfo origin = body_[person];

	TasksInfo untouched_tasks = origin;
	untouched_tasks.erase(TaskStatus::DONE);

	TasksInfo updated_tasks;
	updated_tasks[TaskStatus::NEW] = 0;

	if (task_count == 0)
		return tie(updated_tasks, updated_tasks);

	int task_numb = min(task_count, GetNumOfUndoneTasks(untouched_tasks));
	TaskStatus cur = TaskStatus::NEW;
	TaskStatus next = TaskStatus::IN_PROGRESS;

	while (task_numb > 0) {
		// Update tasks while possible (untill we reach ALREADY updated tasks)
		if (origin[cur] > updated_tasks[cur]) {
			origin[cur]--;
			origin[next]++;
			untouched_tasks[cur]--;
			updated_tasks[next]++;
			task_numb--;
		}
		// Go to text status of tasks
		else {
			cur = next;
			next = (TaskStatus)((int)next + 1);
		}
	}

	ClearMap(origin);
	ClearMap(untouched_tasks);
	ClearMap(updated_tasks);

	body_[person] = origin;
	return tie(updated_tasks, untouched_tasks);
}

int TeamTasks::GetNumOfUndoneTasks(map<TaskStatus, int> & tasks) const
{
	int undone_num = 0;
	for_each(tasks.begin(), tasks.end(), [&undone_num](auto p) { undone_num += p.second; });

	return undone_num;
}

void TeamTasks::ClearMap(TasksInfo & item)
{
	TasksInfo tmp = item;
	for_each(tmp.begin(), tmp.end(), [& item](auto p) 
	{
		if (p.second == 0)
			item.erase(p.first);
	});
}
