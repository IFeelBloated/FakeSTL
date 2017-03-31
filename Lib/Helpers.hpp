#pragma once
#include <new>
#include <iostream>
#include <initializer_list>
#include <cstdint>
#include <cstring>

#if defined(_MSC_VER)
#include <malloc.h>
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#include <alloca.h>
#endif

namespace StandardTemplateLibrary {
	constexpr auto operator""_size(unsigned long long Value) {
		return static_cast<size_t>(Value);
	}
}