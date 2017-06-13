#pragma once
#include "Helpers.hpp"

namespace StandardTemplateLibrary {
	template<typename GenericType>
	struct RemoveReference final {
		using Type = GenericType;
		RemoveReference() = delete;
		RemoveReference(RemoveReference &&) = delete;
		RemoveReference(const RemoveReference &) = delete;
		auto &operator=(RemoveReference &&) = delete;
		auto &operator=(const RemoveReference &) = delete;
		~RemoveReference() = delete;
	};

	template<typename GenericType>
	struct RemoveReference<GenericType &> final {
		using Type = GenericType;
		RemoveReference() = delete;
		RemoveReference(RemoveReference &&) = delete;
		RemoveReference(const RemoveReference &) = delete;
		auto &operator=(RemoveReference &&) = delete;
		auto &operator=(const RemoveReference &) = delete;
		~RemoveReference() = delete;
	};

	template<typename GenericType>
	struct RemoveReference<GenericType &&> final {
		using Type = GenericType;
		RemoveReference() = delete;
		RemoveReference(RemoveReference &&) = delete;
		RemoveReference(const RemoveReference &) = delete;
		auto &operator=(RemoveReference &&) = delete;
		auto &operator=(const RemoveReference &) = delete;
		~RemoveReference() = delete;
	};

	template<typename GenericType>
	using RemoveReference_t = typename RemoveReference<GenericType>::Type;
}