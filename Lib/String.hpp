#pragma once
#include "Helpers.hpp"

namespace StandardTemplateLibrary {
	template<typename T>
	class BasicString final {
		T *Pointer = nullptr;
		decltype(0_size) Length = 0;
		static auto Compare(const BasicString &ObjectA, const BasicString &ObjectB, bool LessThan) {
			auto ActualCompare = [&](auto &&Operation) {
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
		using BasicStringIterator = T *;
		using ConstantBasicStringIterator = const T *;
		static constexpr auto NPOS = static_cast<std::size_t>(-1);
		BasicString() = default;
		BasicString(std::size_t Count, T Value) {
			Pointer = new T[Count + 1];
			for (auto i = 0_size; i < Count; ++i)
				Pointer[i] = Value;
			Pointer[Count] = 0;
			Length = Count;
		}
		BasicString(const T *Value, std::size_t Length) {
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
		auto begin() {
			return BasicStringIterator{ Pointer };
		}
		auto begin() const {
			return ConstantBasicStringIterator{ Pointer };
		}
		auto end() {
			return BasicStringIterator{ Pointer + Length };
		}
		auto end() const {
			return ConstantBasicStringIterator{ Pointer + Length };
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
		auto Substring(std::size_t Position, std::size_t Length = NPOS) const {
			if (Position + Length > this->Length || Length == NPOS)
				Length = this->Length - Position;
			return BasicString{ Pointer + Position, Length };
		}
		auto Find(const BasicString &Object, std::size_t Position = 0) const {
			auto GetMaximum = [](auto x, auto y) {
				return x < y ? y : x;
			};
			auto ShiftTable = reinterpret_cast<std::size_t *>(alloca(GetMaximum(Object.Length, 2) * sizeof(std::size_t)));
			auto InitializeShiftTable = [&]() {
				auto Position = 2_size;
				auto Cursor = 0_size;
				auto RecordCursorAndMoveOn = [&]() {
					ShiftTable[Position] = Cursor + 1;
					++Position;
					++Cursor;
				};
				auto SkipAndMoveOn = [&]() {
					ShiftTable[Position] = 0;
					++Position;
				};
				auto RelocateCursor = [&]() {
					Cursor = ShiftTable[Cursor];
				};
				ShiftTable[0] = NPOS;
				ShiftTable[1] = 0;
				while (Position < Object.Length)
					if (Object[Position - 1] == Object[Cursor])
						RecordCursorAndMoveOn();
					else if (Cursor > 0)
						RelocateCursor();
					else
						SkipAndMoveOn();
			};
			auto OptimizeShiftTable = [&]() {
				for (auto i = 1_size; i < Object.Length; ++i)
					if (Object[i] == Object[ShiftTable[i]])
						ShiftTable[i] = ShiftTable[ShiftTable[i]];
			};
			auto ActualFind = [&]() {
				auto Cursor = 0_size;
				auto &Self = *this;
				auto MoveOn = [&]() {
					++Position;
					++Cursor;
				};
				auto RelocateCursor = [&]() {
					auto ResetCursorAndMoveOn = [&]() {
						++Position;
						Cursor = 0;
					};
					Cursor = ShiftTable[Cursor];
					if (Cursor == NPOS)
						ResetCursorAndMoveOn();
				};
				while (Cursor < Object.Length && Position < Length)
					if (Self[Position] == Object[Cursor])
						MoveOn();
					else
						RelocateCursor();
				if (Cursor == Object.Length)
					return Position - Object.Length;
				else
					return NPOS;
			};
			InitializeShiftTable();
			OptimizeShiftTable();
			return ActualFind();
		}
		auto &operator[](std::size_t Position) {
			return Pointer[Position];
		}
		const auto &operator[](std::size_t Position) const {
			return Pointer[Position];
		}
		auto &operator+=(const BasicString &Object) {
			if (Object.Length > 0) {
				auto NewPointer = new T[Length + Object.Length + 1];
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
			return BasicString{ ObjectA } += ObjectB;
		}
		friend auto operator+(BasicString &&ObjectA, const BasicString &ObjectB) {
			return ObjectA += ObjectB;
		}
		friend auto operator==(const BasicString &ObjectA, const BasicString &ObjectB) {
			auto ElementWiseCompare = [&]() {
				for (auto i = 0_size; i < ObjectA.Length; ++i)
					if (ObjectA[i] == ObjectB[i])
						continue;
					else
						return false;
				return true;
			};
			if (ObjectA.Length != ObjectB.Length)
				return false;
			else
				return ElementWiseCompare();
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
		friend auto &operator<<(std::basic_ostream<T, std::char_traits<T>> &Output, const BasicString &Object) {
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
			auto operator""s(const char *Value, std::size_t Length) {
				return String{ Value, Length };
			}

			auto operator""s(const char16_t *Value, std::size_t Length) {
				return U16String{ Value, Length };
			}

			auto operator""s(const char32_t *Value, std::size_t Length) {
				return U32String{ Value, Length };
			}

			auto operator""s(const wchar_t *Value, std::size_t Length) {
				return WString{ Value, Length };
			}
		}
	}
}
