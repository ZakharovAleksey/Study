#ifndef HEADER_WEEK_1_H
#define HEADER_WEEK_1_H

#include<iostream>
#include <cstdlib>
#include<vector>
#include<map>
#include <string>
#include<utility>
#include<algorithm>
#include<tuple> 
#include <stdexcept> 

using namespace std;

#pragma region Трекер задач

	/*
		Реализуйте класс TeamTasks, позволяющий хранить статистику по статусам задач команды разработчиков.
	*/

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
		tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count);

		int GetNumOfUndoneTasks(map<TaskStatus, int>& tasks) const;

		void ClearMap(TasksInfo& item);

	private:
		std::map<std::string, TasksInfo> body_;
	};

#pragma endregion

#pragma region Всё в квадрат

/*
	Реализуйте шаблонную функцию Sqr, которая работает не только для чисел, но и для контейнеров. 
	Функция должна возвращать копию исходного контейнера, модифицировав его следующим образом:
	- для vector элементы нужно возвести в квадрат;
	- для map в квадрат нужно возвести только значения, но не ключи;
	- для pair в квадрат нужно возвести каждый элемент пары.
*/

template<typename T> T Sqr(T val);
template<typename T> std::vector<T> Sqr(std::vector<T> & v);
template<typename Key, typename Val> std::map<Key, Val> Sqr(std::map<Key, Val> m);
template<typename Key, typename Val> std::pair<Key, Val> Sqr(std::pair<Key, Val> & p);

template<typename T>
T Sqr(T val)
{
	return val * val;
}

template<typename T>
std::vector<T> Sqr(std::vector<T> & v)
{
	std::vector<T> tmp = v;
	for (auto & i : tmp)
		i = Sqr(i);
	return tmp;
}

template<typename Key, typename Val>
std::map<Key, Val> Sqr(std::map<Key, Val> m)
{
	std::map<Key, Val> tmp = m;
	for (auto & i : tmp)
	{
		i.second = Sqr(i.second);
	}
	return tmp;
}

template<typename Key, typename Val>
std::pair<Key, Val> Sqr(std::pair<Key, Val> & p)
{
	return std::make_pair(Sqr(p.first), Sqr(p.second));
}

#pragma endregion

#pragma region Ссылка на элемент

/*
	Реализуйте шаблонную функцию GetRefStrict, которая на вход принимает: map и значение ключа k. 
	Если элемент по ключу k в коллекции отсутствует, то функция должна бросить исключение runtime_error, иначе вернуть ссылку на элемент в коллекции.
*/

template<class Key, class Val>
Val & GetRefStrict(std::map<Key, Val> & m, Key key)
{
	if (m.count(key) == 0)
		throw std::runtime_error("There is no such key in map");
	return m[key];	
}

#pragma endregion

#endif // ! HEADER_WEEK_1_H
