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
	struct RemoveConst final {
		using Type = GenericType;
		RemoveConst() = delete;
		RemoveConst(RemoveConst &&) = delete;
		RemoveConst(const RemoveConst &) = delete;
		auto &operator=(RemoveConst &&) = delete;
		auto &operator=(const RemoveConst &) = delete;
		~RemoveConst() = delete;
	};

	template<typename GenericType>
	struct RemoveConst<const GenericType> final {
		using Type = GenericType;
		RemoveConst() = delete;
		RemoveConst(RemoveConst &&) = delete;
		RemoveConst(const RemoveConst &) = delete;
		auto &operator=(RemoveConst &&) = delete;
		auto &operator=(const RemoveConst &) = delete;
		~RemoveConst() = delete;
	};

	template<typename GenericType>
	struct RemoveVolatile final {
		using Type = GenericType;
		RemoveVolatile() = delete;
		RemoveVolatile(RemoveVolatile &&) = delete;
		RemoveVolatile(const RemoveVolatile &) = delete;
		auto &operator=(RemoveVolatile &&) = delete;
		auto &operator=(const RemoveVolatile &) = delete;
		~RemoveVolatile() = delete;
	};

	template<typename GenericType>
	struct RemoveVolatile<volatile GenericType> final {
		using Type = GenericType;
		RemoveVolatile() = delete;
		RemoveVolatile(RemoveVolatile &&) = delete;
		RemoveVolatile(const RemoveVolatile &) = delete;
		auto &operator=(RemoveVolatile &&) = delete;
		auto &operator=(const RemoveVolatile &) = delete;
		~RemoveVolatile() = delete;
	};

	template<typename GenericType>
	struct RemoveCV final {
		using Type = RemoveVolatile_t<RemoveConst_t<GenericType>>;
		RemoveCV() = delete;
		RemoveCV(RemoveCV &&) = delete;
		RemoveCV(const RemoveCV &) = delete;
		auto &operator=(RemoveCV &&) = delete;
		auto &operator=(const RemoveCV &) = delete;
		~RemoveCV() = delete;
	};

	template<typename GenericType>
	using RemoveReference_t = typename RemoveReference<GenericType>::Type;

	template<typename GenericType>
	using RemoveConst_t = typename RemoveConst<GenericType>::Type;

	template<typename GenericType>
	using RemoveVolatile_t = typename RemoveVolatile<GenericType>::Type;

	template<typename GenericType>
	using RemoveCV_t = typename RemoveCV<GenericType>::Type;
}