#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

using namespace std;
using namespace unit_test;
using namespace log_time;


#include<list>
#include<array>
const size_t MAX_SIZE = 1'000'000u;

// https://github.com/lazyhacker/gapbuffer
class Editor {
public:
	Editor() : cur_buffer_len_(0u), cur_pos_(text_.begin()), cur_id_(0u) {}

	
	void Left() {
		if (cur_pos_ != text_.begin()) {
			cur_pos_ = prev(cur_pos_);
			--cur_id_;
		}
	}
	void Right() {
		if (cur_pos_ != prev(text_.end())) {
			cur_pos_ = next(cur_pos_);
			++cur_id_;
		}
	}
	void Insert(char token) {
		cur_pos_ = text_.insert(cur_pos_, token);
	}
	void Copy(size_t tokens) {
		cur_buffer_len_ = min(tokens, text_.size() - cur_id_);
		//copy()
	}

	//void Cut(size_t tokens);  // вырезать не более tokens символов,
	//						  // начиная с текущей позиции курсора
	//void Paste();  // вставить содержимое буфера
	//			   // в текущую позицию курсора
	//string GetText() const;  // получить текущее содержимое
	//						 // текстового редактора

private:
	array<char, MAX_SIZE> buffer_;
	array<char, MAX_SIZE>::iterator buffer_pos_;
	size_t cur_buffer_len_;


	list<char> text_;
	list<char>::iterator cur_pos_;
	size_t cur_id_;
};

int main() {

	cout << "DONE\n";
	return 0;
}