#pragma once

#include "../common/uncopyable.h"

#include <list>
#include <functional>


BEGIN_NAMESPACE(WndDesign)


template<class T>
class State : Uncopyable {
public:
	class Watcher : Uncopyable {
	private:
		friend class State;
	public:
		Watcher(State& state, std::function<void(const T&)> callback) : state(&state), callback(callback) { this->state->AddWatcher(*this); }
		Watcher(Watcher& watcher, std::function<void(const T&)> callback) : state(watcher.state), callback(callback) { this->state->AddWatcher(*this); }
		~Watcher() { if (state) { state->RemoveWatcher(*this); } }
	private:
		ref_ptr<State> state;
		std::list<ref_ptr<Watcher>>::iterator index = {};
		std::function<void(const T&)> callback;
	private:
		void Callback() { if (callback) { callback(Get()); } }
	public:
		const T& Get() const { return state->Get(); }
	};
public:
	State(T value = {}) : value(std::move(value)) {}
	~State() { for (auto& watcher : watcher_list) { if (watcher) { watcher->state = nullptr; } } }
private:
	T value;
	std::list<ref_ptr<Watcher>> watcher_list;
private:
	void AddWatcher(Watcher& watcher) {
		watcher_list.emplace_front(&watcher);
		watcher.index = watcher_list.begin();
	}
	void RemoveWatcher(Watcher& watcher) {
		*(watcher.index) = nullptr;
		watcher.index = {};
	}
public:
	const T& Get() const {
		return value;
	}
	void Set(T value) {
		this->value = std::move(value);
		for (auto it = watcher_list.begin(); it != watcher_list.end();) {
			if (*it == nullptr) { it = watcher_list.erase(it); continue; }
			(*it++)->Callback();
		}
	}
	const T& operator=(T value) {
		Set(std::move(value));
		return Get();
	}
};


END_NAMESPACE(WndDesign)