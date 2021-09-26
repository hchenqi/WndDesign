#pragma once

#include <stdexcept>
#include <cassert>


#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name)   }
#define Anonymous

#define ABSTRACT_BASE _declspec(novtable)
#define pure = 0


BEGIN_NAMESPACE(WndDesign)


template<class T> using ref_ptr = T*;
template<class T> using alloc_ptr = T*;

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using uint64 = unsigned long long;
using wchar = wchar_t;


template<class T> constexpr T max(T a, T b) { return a > b ? a : b; }
template<class T> constexpr T min(T a, T b) { return a < b ? a : b; }


END_NAMESPACE(WndDesign)