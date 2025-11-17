#pragma once

#include <cstdint>
#include <climits>

using uint16 = uint16_t;
using uint  = uint32_t;
using uint64 = uint64_t;

template<typename T>
struct Rect
{
	T X;
	T Y;
	T Width;
	T Height;
};

template<typename T>
struct Color
{
	T R;
	T G;
	T B;
	T A;
};