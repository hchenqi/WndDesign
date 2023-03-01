#pragma once

#include "core.h"

#include <tuple>


BEGIN_NAMESPACE(WndDesign)


template<class Func>
struct function_traits : public function_traits<decltype(&Func::operator())> {};

template<class Ret, class... Args>
struct function_traits<Ret(Args...)> {
	using argument_type_tuple = std::tuple<Args...>;
	using return_type = Ret;
};

template<class Ret, class... Args>
struct function_traits<Ret(*)(Args...)> : public function_traits<Ret(Args...)> {};

template<class Ret, class Class, class... Args>
struct function_traits<Ret(Class::*)(Args...)> : public function_traits<Ret(Args...)> {};

template<class Ret, class Class, class... Args>
struct function_traits<Ret(Class::*)(Args...) const> : public function_traits<Ret(Args...)> {};


END_NAMESPACE(WndDesign)