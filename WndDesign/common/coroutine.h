#pragma once

#include "uncopyable.h"
#include "function_traits.h"

#include <coroutine>
#include <functional>


BEGIN_NAMESPACE(WndDesign)


template<class T = void>
struct Task : Uncopyable {
private:
	template<class T> struct promise;
public:
	using promise_type = promise<T>;

private:
	Task(std::coroutine_handle<promise_type> continuation) : continuation(continuation) { Promise().SetTask(this); }
public:
	Task(Task&& task) : continuation(task.continuation) { Promise().SetTask(this); task.continuation = nullptr; }
	void operator=(Task&& task) { continuation = task.continuation; Promise().SetTask(this); task.continuation = nullptr; }
	~Task() { if (continuation) { Promise().SetTask(nullptr); } }

private:
	std::coroutine_handle<promise_type> continuation;
private:
	promise_type& Promise() const { return continuation.promise(); }
public:
	bool await_ready() { return continuation == nullptr; }
	void await_suspend(std::coroutine_handle<> continuation) { Promise().SetOuterContinuation(continuation); }
	T await_resume() { return Promise().GetValue(); }

private:
	template<class T> requires (std::is_void_v<T>)
	struct promise<T> {
	private:
		ref_ptr<Task> task = nullptr;
		std::coroutine_handle<> outer_continuation = nullptr;
	public:
		~promise() { if (task) { task->continuation = nullptr; } }
	public:
		void SetTask(ref_ptr<Task> task) { this->task = task; }
		void SetOuterContinuation(std::coroutine_handle<> outer_continuation) { this->outer_continuation = outer_continuation; }
	public:
		Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }
		auto initial_suspend() noexcept { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_never{}; }
		void unhandled_exception() {}
		void return_void() { if (outer_continuation) { outer_continuation(); outer_continuation = nullptr; } }
	public:
		void GetValue() {}
	};

	template<class T> requires (!std::is_void_v<T>)
	struct promise<T> {
	private:
		ref_ptr<Task> task = nullptr;
		std::coroutine_handle<> outer_continuation = nullptr;
		T value;
	public:
		~promise() { if (task) { task->continuation = nullptr; } }
	public:
		void SetTask(ref_ptr<Task> task) { this->task = task; }
		void SetOuterContinuation(std::coroutine_handle<> outer_continuation) { this->outer_continuation = outer_continuation; }
	public:
		Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(*this)); }
		auto initial_suspend() noexcept { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_never{}; }
		void unhandled_exception() {}
		void return_value(T value) { this->value = std::move(value); if (outer_continuation) { outer_continuation(); outer_continuation = nullptr; } }
	public:
		T GetValue() { return std::move(value); }
	};
};


template<class T = void>
using resolver = std::function<void(T)>;

template<class T>
struct _Task_Awaitable;

template<class T> requires (std::is_void_v<T>)
struct _Task_Awaitable<T> {
	std::function<void(resolver<T>)> task;
	constexpr bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<> continuation) { task(continuation); }
	constexpr void await_resume() {}
};

template<class T> requires (!std::is_void_v<T>)
struct _Task_Awaitable<T> {
	std::function<void(resolver<T>)> task;
	T value;
	constexpr bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<> continuation) { task([this, continuation](T value) { this->value = std::move(value); continuation(); }); }
	T await_resume() { return std::move(value); }
};

template<class Func>
struct _argument_type;

template<class Func> requires (std::tuple_size_v<typename function_traits<Func>::argument_type_tuple> == 0 && std::is_void_v<typename function_traits<Func>::return_type>)
struct _argument_type<Func> {
	using type = void;
};

template<class Func> requires (std::tuple_size_v<typename function_traits<Func>::argument_type_tuple> == 1 && std::is_void_v<typename function_traits<Func>::return_type>)
struct _argument_type<Func> {
	using type = std::tuple_element_t<0, typename function_traits<Func>::argument_type_tuple>;
};

template<class Func>
using _argument_type_t = typename _argument_type<Func>::type;

template<class Func>
Task<_argument_type_t<_argument_type_t<Func>>> StartTask(Func task) {
	co_return co_await _Task_Awaitable<_argument_type_t<_argument_type_t<Func>>>{task};
}


END_NAMESPACE(WndDesign)