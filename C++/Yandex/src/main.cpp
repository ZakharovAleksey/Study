#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

#include <cstdint>
#include<memory>


template <typename T>
class PriorityCollection {
public:
	using Priority = size_t;
	using Id = size_t;
	using ObjPos = std::vector<T>::iterator;

	static const size_t N = 1'000'000u;
	
	Id Add(T object) {
		if (d_priorityHeap.empty()) {
			d_priorityHeap[0] = move(object);
		}
		else {
			d_priorityHeap[0].push_back(move(object));
		}
		d_IdIdentifier[curId++] = make_pair(0, prev(end(d_priorityHeap[0])));
	}

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
		if (d_priorityHeap.empty()) {
			d_priorityHeap[0]{
				make_move_iterator(range_begin),
				make_move_iterator(range_end),
				back_inserter(d_priorityHeap[0])
			}
		}
		else {
			
		}
	}

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const;

	// Получить объект по идентификатору
	const T& Get(Id id) const;

	// Увеличить приоритет объекта на 1
	void Promote(Id id);

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, int> GetMax() const;

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax();

private:
	// heap: [prioruty] {vector<objects>} last with Priority is in push_back
	map<Priority, vector<T>> d_priorityHeap;
	// Here index id -> lead to object pair<> Priorith and position in Priority vector
	array<pair<Priority, ObjPos>, N> d_IdIdentifier;
	size_t curId{ 0 };

};


int main() {
	TestRunner tr;

	vector<int> v = { 1, 4, 2, 3, 5 };
	auto iter = v.begin();
	make_heap(begin(v), end(v));
	copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
	cout << endl;
	cout << *iter << endl;

	return 0;
}