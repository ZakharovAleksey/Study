#ifndef HEADER_WEEK_1_H
#define HEADER_WEEK_1_H

// ������� tie() ��������� ��� ��������� ��������� ��������� (��� ����� ������, ������� ��� �����������)
// ������� tuple - make_tuple()
// ������ � �������� get<1>(tuple_name)
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
		// �������� ���������� �� �������� ����� ����������� ������������
		const TasksInfo& GetPersonTasksInfo(const string& person) const;

		// �������� ����� ������ (� ������� NEW) ��� ����������� �������������
		void AddNewTask(const string& person);

		// �������� ������� �� ������� ���������� ����� ����������� ������������,
		// ����������� ��. ����
		tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count);

	private:
		std::map<std::string, TasksInfo> body_;
	};

}

#pragma endregion


#endif // ! HEADER_WEEK_1_H
