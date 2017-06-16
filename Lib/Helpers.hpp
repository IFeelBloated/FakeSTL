#pragma once
#include <new>
#include <iostream>
#include <initializer_list>
#include <cstddef>
#include <cstdlib>
#include <cwchar>
#include <cstdint>
#include <cstring>
#include <cmath>

#if defined(_MSC_VER)
#include <malloc.h>
#elif defined(__GNUC__) || defined(__GNUG__)
#include <alloca.h>
#endif

namespace StandardTemplateLibrary {
	constexpr auto operator""_size(unsigned long long Value) {
		return static_cast<std::size_t>(Value);
	}

	auto CastToConstantPointer = [](const auto *Pointer) {
		return Pointer;
	};
}