#pragma once

#include "core.h"

#include <array>
#include <vector>


BEGIN_NAMESPACE(WndDesign)


inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);


template<class T, size_t extent>
struct array_vector_selector {
	using type = std::array<T, extent>;

	template<class... Types>
	static std::enable_if_t<sizeof...(Types) == extent, std::array<T, extent>> construct(Types... args) {
		return { static_cast<T>(std::forward<Types>(args))... };
	}
};

template<class T>
struct array_vector_selector<T, dynamic_extent> {
	using type = std::vector<T>;

	template<class... Types>
	static std::vector<T> construct(Types... args) {
		T array[] = { static_cast<T>(std::forward<Types>(args))... };
		return std::vector<T>(std::make_move_iterator(std::begin(array)), std::make_move_iterator(std::end(array)));
	}
};

template<class T, size_t extent>
using array_vector_selector_t = typename array_vector_selector<T, extent>::type;


END_NAMESPACE(WndDesign)