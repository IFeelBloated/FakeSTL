#pragma once
#include <new>
#include <iostream>
#include <cstring>
#include <cstdint>

#if defined(_MSC_VER)
#include <malloc.h>
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#include <alloca.h>
#endif

namespace StandardTemplateLibrary {
	constexpr auto operator""_size(unsigned long long Value) {
		return static_cast<size_t>(Value);
	}

	template<typename T>
	class BasicStringIterator final {
		T *Pointer = nullptr;
	public:
		BasicStringIterator() = delete;
		BasicStringIterator(T *Item) {
			Pointer = Item;
		}
		BasicStringIterator(const BasicStringIterator &) = default;
		BasicStringIterator(BasicStringIterator &&) = default;
		auto operator=(const BasicStringIterator &)->BasicStringIterator & = default;
		auto operator=(BasicStringIterator &&)->BasicStringIterator & = default;
		~BasicStringIterator() = default;
		auto &operator*() const {
			return *Pointer;
		}
		auto &operator[](intptr_t Position) const {
			return Pointer[Position];
		}
		auto &operator+=(intptr_t Value) {
			Pointer += Value;
			return *this;
		}
		auto &operator-=(intptr_t Value) {
			Pointer -= Value;
			return *this;
		}
		auto &operator++() {
			++Pointer;
			return *this;
		}
		auto &operator--() {
			--Pointer;
			return *this;
		}
		auto operator==(const BasicStringIterator &Object) const {
			return Pointer == Object.Pointer;
		}
		auto operator!=(const BasicStringIterator &Object) const {
			return Pointer != Object.Pointer;
		}
		friend auto operator-(const BasicStringIterator &Object, intptr_t Value) {
			auto TemporaryObject = BasicStringIterator{ Object };
			return  TemporaryObject -= Value;
		}
		friend auto operator-(BasicStringIterator &&Object, intptr_t Value) {
			return Object -= Value;
		}
		friend auto operator+(const BasicStringIterator &Object, intptr_t Value) {
			auto TemporaryObject = BasicStringIterator{ Object };
			return  TemporaryObject += Value;
		}
		friend auto operator+(BasicStringIterator &&Object, intptr_t Value) {
			return Object += Value;
		}
		friend auto operator+(intptr_t Value, const BasicStringIterator &Object) {
			auto TemporaryObject = BasicStringIterator{ Object };
			return  TemporaryObject += Value;
		}
		friend auto operator+(intptr_t Value, BasicStringIterator &&Object) {
			return Object += Value;
		}
		friend auto operator-(const BasicStringIterator &ObjectA, const BasicStringIterator &ObjectB) {
			return reinterpret_cast<intptr_t>(ObjectA.Pointer) - reinterpret_cast<intptr_t>(ObjectB.Pointer);
		}
	};

	template<typename T>
	class BasicString final {
		T *Pointer = nullptr;
		size_t Length = 0;
		static constexpr auto NPOS = static_cast<size_t>(-1);
		static auto Compare(const BasicString &ObjectA, const BasicString &ObjectB, bool LessThan) {
			auto ActualCompare = [&](auto Operation) {
				auto GetMinimum = [](auto x, auto y) {
					return x > y ? y : x;
				};
				auto i = 0_size;
				while (i < GetMinimum(ObjectA.Length, ObjectB.Length) && ObjectA[i] == ObjectB[i])
					++i;
				return Operation(ObjectA[i], ObjectB[i]);
			};
			if (LessThan)
				return ActualCompare([](auto x, auto y) {
				return x < y;
			});
			else
				return ActualCompare([](auto x, auto y) {
				return x > y;
			});
		}
	public:
		BasicString() = default;
		BasicString(size_t Count, T Value) {
			Pointer = new T[Count + 1];
			for (auto i = 0_size; i < Count; ++i)
				Pointer[i] = Value;
			Pointer[Count] = 0;
			Length = Count;
		}
		BasicString(const T *Value, size_t Length) {
			this->Length = Length;
			Pointer = new T[Length + 1];
			std::memcpy(Pointer, Value, Length * sizeof(T));
			Pointer[Length] = 0;
		}
		BasicString(const T *Value) :BasicString{ Value, [&]() {
			auto i = 0_size;
			while (Value[i] != 0)
				++i;
			return i;
		}() } {}
		BasicString(const BasicString &Object) {
			*this = Object;
		}
		BasicString(BasicString &&Object) {
			*this = static_cast<BasicString &&>(Object);
		}
		auto &operator=(const BasicString &Object) {
			if (this != &Object) {
				this->~BasicString();
				new(this) BasicString{ Object.Pointer, Object.Length };
			}
			return *this;
		}
		auto &operator=(BasicString &&Object) {
			if (this != &Object) {
				auto TemporaryPointer = Pointer;
				Pointer = Object.Pointer;
				Length = Object.Length;
				Object.Pointer = TemporaryPointer;
			}
			return *this;
		}
		~BasicString() {
			delete[] Pointer;
		}
		auto Clear() {
			this->~BasicString();
			new(this) BasicString{};
		}
		auto Empty() const {
			return Length == 0;
		}
		auto begin() const {
			return BasicStringIterator<T>{ Pointer };
		}
		auto end() const {
			return BasicStringIterator<T>{ Pointer + Length };
		}
		auto Size() const {
			return Length;
		}
		auto Data() {
			return Pointer;
		}
		const auto Data() const {
			return Pointer;
		}
		auto Substring(size_t Position, size_t Length = NPOS) const {
			if (Position + Length > this->Length || Length == NPOS)
				Length = this->Length - Position;
			return BasicString{ Pointer + Position, Length };
		}
		auto Find(const BasicString &Object, size_t Position = 0) const {
			auto GetMaximum = [](auto x, auto y) {
				return x < y ? y : x;
			};
			auto ShiftTable = reinterpret_cast<size_t *>(alloca(GetMaximum(Object.Length, 1) * sizeof(size_t)));
			auto InitializeShiftTable = [&]() {
				auto CalculateShiftValue = [&](auto Position) {
					auto Cursor = Position;
					ShiftTable[Position] = 0;
					while (static_cast<long long>(ShiftTable[Cursor - 1]) >= 0)
						if (Object[Cursor - 1] == Object[ShiftTable[Cursor - 1]]) {
							++ShiftTable[Position];
							--Cursor;
						}
						else
							break;
				};
				auto OptimizeShiftValue = [&](auto Position) {
					if (Object[Position] == Object[ShiftTable[Position]])
						ShiftTable[Position] = ShiftTable[ShiftTable[Position]];
				};
				ShiftTable[0] = NPOS;
				for (auto i = 1_size; i < Object.Length; ++i) {
					CalculateShiftValue(i);
					OptimizeShiftValue(i);
				}
			};
			auto ActualFind = [&]() {
				auto Cursor = 0_size;
				auto &Self = *this;
				while (Cursor < Object.Length && Position < Length)
					if (Self[Position] == Object[Cursor]) {
						++Position;
						++Cursor;
					}
					else {
						Cursor = ShiftTable[Cursor];
						if (Cursor == NPOS) {
							++Position;
							Cursor = 0;
						}
					}
				if (Cursor == Object.Length)
					return Position - Object.Length;
				else
					return NPOS;
			};
			InitializeShiftTable();
			return ActualFind();
		}
		auto &operator[](size_t Position) {
			return Pointer[Position];
		}
		const auto &operator[](size_t Position) const {
			return Pointer[Position];
		}
		auto &operator+=(const BasicString &Object) {
			if (Object.Length > 0) {
				auto NewPointer = new T[Length + Object.Length];
				std::memcpy(NewPointer, Pointer, Length * sizeof(T));
				std::memcpy(NewPointer + Length, Object.Pointer, Object.Length * sizeof(T));
				Length += Object.Length;
				NewPointer[Length] = 0;
				delete[] Pointer;
				Pointer = NewPointer;
			}
			return *this;
		}
		friend auto operator+(const BasicString &ObjectA, const BasicString &ObjectB) {
			auto TemporaryObject = BasicString{ ObjectA };
			return TemporaryObject += ObjectB;
		}
		friend auto operator+(BasicString &&ObjectA, const BasicString &ObjectB) {
			return ObjectA += ObjectB;
		}
		friend auto operator==(const BasicString &ObjectA, const BasicString &ObjectB) {
			if (ObjectA.Length != ObjectB.Length)
				return false;
			else {
				for (auto i = 0_size; i < ObjectA.Length; ++i)
					if (ObjectA[i] == ObjectB[i])
						continue;
					else
						return false;
				return true;
			}
		}
		friend auto operator!=(const BasicString &ObjectA, const BasicString &ObjectB) {
			return !(ObjectA == ObjectB);
		}
		friend auto operator<(const BasicString &ObjectA, const BasicString &ObjectB) {
			return Compare(ObjectA, ObjectB, true);
		}
		friend auto operator>(const BasicString &ObjectA, const BasicString &ObjectB) {
			return Compare(ObjectA, ObjectB, false);
		}
		friend auto operator<=(const BasicString &ObjectA, const BasicString &ObjectB) {
			return !(ObjectA > ObjectB);
		}
		friend auto operator>=(const BasicString &ObjectA, const BasicString &ObjectB) {
			return !(ObjectA < ObjectB);
		}
		friend auto &operator<<(std::ostream &Output, const BasicString &Object) {
			if (Object.Pointer != nullptr)
				Output << Object.Pointer;
			return Output;
		}
	};

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
	using U16String = BasicString<char16_t>;
	using U32String = BasicString<char32_t>;

	inline namespace Literals {
		inline namespace StringLiterals {
			auto operator""s(const char *Value, size_t Length) {
				return String{ Value, Length };
			}

			auto operator""s(const char16_t *Value, size_t Length) {
				return U16String{ Value, Length };
			}

			auto operator""s(const char32_t *Value, size_t Length) {
				return U32String{ Value, Length };
			}

			auto operator""s(const wchar_t *Value, size_t Length) {
				return WString{ Value, Length };
			}
		}
	}
}

