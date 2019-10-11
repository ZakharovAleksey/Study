#ifndef HEADER_WEEK_1_H
#define HEADER_WEEK_1_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

#pragma region Task 1

/*
        ���������� ����� TeamTasks, ����������� ������� ���������� �� ��������
   ����� ������� �������������.
*/

enum class TaskStatus
{
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
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person,
                                                 int task_count);

  int GetNumOfUndoneTasks(map<TaskStatus, int>& tasks) const;

  void ClearMap(TasksInfo& item);

  private:
  std::map<std::string, TasksInfo> body_;
};

#pragma endregion

#pragma region �� � �������

/*
        ���������� ��������� ������� Sqr, ������� �������� �� ������ ��� �����,
   �� � ��� �����������. ������� ������ ���������� ����� ��������� ����������,
   ������������� ��� ��������� �������:
        - ��� vector �������� ����� �������� � �������;
        - ��� map � ������� ����� �������� ������ ��������, �� �� �����;
        - ��� pair � ������� ����� �������� ������ ������� ����.
*/

template<typename T>
T Sqr(T val);
template<typename T>
std::vector<T> Sqr(std::vector<T>& v);
template<typename Key, typename Val>
std::map<Key, Val> Sqr(std::map<Key, Val> m);
template<typename Key, typename Val>
std::pair<Key, Val> Sqr(std::pair<Key, Val>& p);

template<typename T>
T Sqr(T val)
{
  return val * val;
}

template<typename T>
std::vector<T> Sqr(std::vector<T>& v)
{
  std::vector<T> tmp = v;
  for (auto& i : tmp)
    i = Sqr(i);
  return tmp;
}

template<typename Key, typename Val>
std::map<Key, Val> Sqr(std::map<Key, Val> m)
{
  std::map<Key, Val> tmp = m;
  for (auto& i : tmp)
  {
    i.second = Sqr(i.second);
  }
  return tmp;
}

template<typename Key, typename Val>
std::pair<Key, Val> Sqr(std::pair<Key, Val>& p)
{
  return std::make_pair(Sqr(p.first), Sqr(p.second));
}

#pragma endregion

#pragma region ������ �� �������

/*
        ���������� ��������� ������� GetRefStrict, ������� �� ���� ���������:
   map � �������� ����� k. ���� ������� �� ����� k � ��������� �����������, ��
   ������� ������ ������� ���������� runtime_error, ����� ������� ������ ��
   ������� � ���������.
*/

template<class Key, class Val>
Val& GetRefStrict(std::map<Key, Val>& m, Key key)
{
  if (m.count(key) == 0)
    throw std::runtime_error("There is no such key in map");
  return m[key];
}

#pragma endregion

#endif  // ! HEADER_WEEK_1_H
