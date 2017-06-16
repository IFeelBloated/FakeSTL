#pragma once
#include "Helpers.hpp"
#include "TypeTraits.hpp"

namespace StandardTemplateLibrary {
	template<typename MovableType>
	constexpr auto &&Move(MovableType &&MovableObject) {
		return static_cast<RemoveReference_t<MovableType> &&>(MovableObject);
	}

	template<typename MovableType>
	auto Swap(MovableType &MovableObjectA, MovableType &MovableObjectB) {
		auto TemporaryObject = Move(MovableObjectA);
		MovableObjectA = Move(MovableObjectB);
		MovableObjectB = Move(TemporaryObject);
	}
}