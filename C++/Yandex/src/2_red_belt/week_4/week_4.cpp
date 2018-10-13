#include"week_4.h"

namespace week_4 {
	
	void Editor::Left() {
		if (start_buf_.size() != 0) {
			end_buf_.push_front(*prev(end(start_buf_)));
			start_buf_.pop_back();
		}
	}

	void Editor::Right() {
		if (end_buf_.size() != 0) {
			start_buf_.push_back(*begin(end_buf_));
			end_buf_.pop_front();
		}
	}

	void Editor::Insert(char token) {
		start_buf_.push_back(token);
	}

	void Editor::Copy(size_t tokens) {
		cur_buf_size_ = min(tokens, end_buf_.size());
		copy(begin(end_buf_), begin(end_buf_) + cur_buf_size_, begin(buffer_));
	}

	void Editor::Cut(size_t tokens) {
		cur_buf_size_ = min(tokens, end_buf_.size());
		copy(begin(end_buf_), begin(end_buf_) + cur_buf_size_, begin(buffer_));

		// Clear cuted part 
		for (size_t i = 0; i < cur_buf_size_; ++i) {
			end_buf_.pop_front();
		}
	}

	void Editor::Paste() {
		for (size_t i = 0; i < cur_buf_size_; ++i) {
			start_buf_.push_back(buffer_[i]);
		}
	}

	string Editor::GetText() const {
		return string(begin(start_buf_), end(start_buf_)) + string(begin(end_buf_), end(end_buf_));
	}


}