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

	/* 
		This region is the correct solution of Translator task, but I have not gon C++ 17 compiler yet :(

	void Translator::Add(string_view source, string_view target) {
		auto for_it = forward_.find(string(source));

		if (for_it == forward_.end()) {
			forward_[string(source)] = string(target);

			auto cur = forward_.find(string(source));
			backward_[string_view(cur->second)] = string_view(cur->first);
		}
		else {
			for_it->second = string(target);

			backward_[string_view(for_it->second)] = string_view(for_it->first);

		}
	}

	string_view Translator::TranslateForward(string_view source) const {
		auto it = forward_.find(string(source));
		if (it == end(forward_)) {
			return string();
		}

		return it->second;
	}

	string_view Translator::TranslateBackward(string_view target) const {
		auto it = backward_.find(target);
		if (it == end(backward_)) {
			return string();
		}

		return it->second;
	}

	*/
}