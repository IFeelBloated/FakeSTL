#pragma once
#include "Helpers.hpp"

namespace StandardTemplateLibrary {
	template<typename MovableObject>
	auto Swap(MovableObject &ObjectA, MovableObject &ObjectB) {
		auto TemporaryObject = static_cast<MovableObject &&>(ObjectA);
		ObjectA = static_cast<MovableObject &&>(ObjectB);
		ObjectB = static_cast<MovableObject &&>(TemporaryObject);
	}
}