#ifndef HEADER_WEEK_1_H
#define HEADER_WEEK_1_H

// Функция tie() связывает для упрощения оператора сравнения (все через ссылки, поэтому нет копирования)
// Создать tuple - make_tuple()
// доступ к элементу get<1>(tuple_name)
#include<iostream>
#include<tuple> 
#include<map>
#include<vector>

using namespace std;

#pragma region Task2

namespace w1_t2
{
	enum class TaskStatus {
		NEW,
		IN_PROGRESS,
		TESTING,
		DONE
	};

	using TasksInfo = map<TaskStatus, int>;

	class TeamTasks {
	public:
		// Получить статистику по статусам задач конкретного разработчика
		const TasksInfo& GetPersonTasksInfo(const string& person) const;

		// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
		void AddNewTask(const string& person);

		// Обновить статусы по данному количеству задач конкретного разработчика,
		// подробности см. ниже
		tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count);

	private:
		std::map<std::string, TasksInfo> body_;
	};

}

#pragma endregion


#endif // ! HEADER_WEEK_1_H
