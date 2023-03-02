#pragma once

#include "uncopyable.h"
#include "function_traits.h"

#include <coroutine>
#include <functional>


BEGIN_NAMESPACE(WndDesign)


template<class T>
struct _Value_Base {
	void SetValue() {}
	T GetValue() {}
};

template<class T> requires (!std::is_void_v<T>)
struct _Value_Base<T> {
	T value;
	void SetValue(T value) { this->value = std::move(value); }
	T GetValue() { return std::move(value); }
};


template<class T = void>
struct Task : private Uncopyable, private _Value_Base<T> {
private:
	template<class T> struct promise;
public:
	using promise_type = promise<T>;

private:
	Task(std::coroutine_handle<promise_type> continuation) : continuation(continuation) { Promise().SetTask(this); }
public:
	Task(Task&& task) : _Value_Base<T>(std::move(task)), continuation(task.continuation) { task.continuation = nullptr; if (continuation) { Promise().SetTask(this); } }
	void operator=(Task&& task) { _Value_Base<T>::operator=(std::move(task)); continuation = task.continuation; task.continuation = nullptr; if (continuation) { Promise().SetTask(this); } }
	~Task() { if (continuation) { Promise().SetTask(nullptr); } }

private:
	std::coroutine_handle<promise_type> continuation;
private:
	promise_type& Promise() const { return continuation.promise(); }
public:
	bool await_ready() { return continuation == nullptr; }
	void await_suspend(std::coroutine_handle<> continuation) { Promise().SetOuterContinuation(continuation); }
	T await_resume() { return _Value_Base<T>::GetValue(); }

private:
	struct promise_base {
	protected:
		ref_ptr<Task> task = nullptr;
		std::coroutine_handle<> outer_continuation = nullptr;
	public:
		~promise_base() { if (task) { task->continuation = nullptr; } if (outer_continuation) { outer_continuation.destroy(); } }
	public:
		void SetTask(ref_ptr<Task> task) { this->task = task; }
		void SetOuterContinuation(std::coroutine_handle<> outer_continuation) { this->outer_continuation = outer_continuation; }
	public:
		Task get_return_object() { return Task(std::coroutine_handle<promise_type>::from_promise(static_cast<promise_type&>(*this))); }
		auto initial_suspend() noexcept { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_never{}; }
		void unhandled_exception() {}
	};

	template<class T> requires (std::is_void_v<T>)
	struct promise<T> : promise_base {
	private:
		promise_base::task;
		promise_base::outer_continuation;
	public:
		void return_void() { if (outer_continuation) { outer_continuation(); outer_continuation = nullptr; } }
	};

	template<class T> requires (!std::is_void_v<T>)
	struct promise<T> : promise_base {
	private:
		promise_base::task;
		promise_base::outer_continuation;
	public:
		void return_value(T value) { if (task) { task->SetValue(std::move(value)); } if (outer_continuation) { outer_continuation(); outer_continuation = nullptr; } }
	};
};


template<class T>
struct _Task_Awaitable;


template<class T = void>
struct Continuation {
public:
	using value_type = T;
private:
	friend struct _Task_Awaitable<T>;
private:
	Continuation(std::function<void(T)> function, std::coroutine_handle<> continuation) : function(function), continuation(continuation) {}
public:
	Continuation() : Continuation(nullptr) {}
	Continuation(nullptr_t) : function(nullptr), continuation(nullptr) {}
private:
	std::function<void(T)> function;
	std::coroutine_handle<> continuation;
public:
	template<class U = T> requires(std::is_void_v<U>) void operator()() const { function(); }
	template<class U = T> requires(!std::is_void_v<U>) void operator()(U value) const { function(std::move(value)); }
	void Destroy() { continuation.destroy(); }
};


template<class T> requires (std::is_void_v<T>)
struct _Task_Awaitable<T> {
	std::function<void(Continuation<T>)> task;
	constexpr bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<> continuation) { task(Continuation<T>(continuation, continuation)); }
	constexpr void await_resume() {}
};

template<class T> requires (!std::is_void_v<T>)
struct _Task_Awaitable<T> {
	std::function<void(Continuation<T>)> task;
	T value;
	constexpr bool await_ready() { return false; }
	void await_suspend(std::coroutine_handle<> continuation) { task(Continuation<T>([this, continuation](T value) { this->value = std::move(value); continuation(); }, continuation)); }
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
Task<typename _argument_type_t<Func>::value_type> StartTask(Func task) {
	co_return co_await _Task_Awaitable<typename _argument_type_t<Func>::value_type>{ task };
}


END_NAMESPACE(WndDesign)