#pragma once
#include "Helpers.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary {
	template<typename CharacterType>
	class BasicString final {
		using Iterator = CharacterType *;
		using ConstantIterator = const CharacterType *;
		CharacterType *StringPointer = nullptr;
		decltype(0_size) Length = 0;
		template<typename UnknownLambdaType>
		static auto FindTheFirstDifferentPairOfCharactersThenCompare(const BasicString &BasicStringA, const BasicString &BasicStringB, UnknownLambdaType &&Action) {
			auto GetMinimum = [](auto x, auto y) {
				return x > y ? y : x;
			};
			auto Cursor = 0_size;
			while (Cursor < GetMinimum(BasicStringA.Length, BasicStringB.Length) && BasicStringA[Cursor] == BasicStringB[Cursor])
				++Cursor;
			return Action(BasicStringA[Cursor], BasicStringB[Cursor]);
		}
	public:
		static constexpr auto NPOS = static_cast<std::size_t>(-1);
		BasicString() = default;
		BasicString(std::size_t Count, CharacterType Character) {
			StringPointer = new CharacterType[Count + 1];
			for (auto Cursor = 0_size; Cursor < Count; ++Cursor)
				StringPointer[Cursor] = Character;
			StringPointer[Count] = 0;
			Length = Count;
		}
		BasicString(const CharacterType *SomeStringPointer, std::size_t Length) {
			this->Length = Length;
			StringPointer = new CharacterType[Length + 1];
			std::memcpy(StringPointer, SomeStringPointer, Length * sizeof(CharacterType));
			StringPointer[Length] = 0;
		}
		BasicString(const CharacterType *SomeStringPointer) :BasicString{ SomeStringPointer, [&]() {
			auto Cursor = 0_size;
			while (SomeStringPointer[Cursor] != 0)
				++Cursor;
			return Cursor;
		}() } {}
		BasicString(const BasicString &OtherBasicString) {
			*this = OtherBasicString;
		}
		BasicString(BasicString &&OtherBasicString) {
			*this = static_cast<BasicString &&>(OtherBasicString);
		}
		auto &operator=(const BasicString &OtherBasicString) {
			if (this != &OtherBasicString) {
				this->~BasicString();
				new(this) BasicString{ OtherBasicString.StringPointer, OtherBasicString.Length };
			}
			return *this;
		}
		auto &operator=(BasicString &&OtherBasicString) {
			if (this != &OtherBasicString) {
				Swap(StringPointer, OtherBasicString.StringPointer);
				Length = OtherBasicString.Length;
			}
			return *this;
		}
		~BasicString() {
			delete[] StringPointer;
		}
		auto Clear() {
			this->~BasicString();
			new(this) BasicString{};
		}
		auto Empty() const {
			return Length == 0;
		}
		auto begin() {
			return Iterator{ StringPointer };
		}
		auto begin() const {
			return ConstantIterator{ StringPointer };
		}
		auto end() {
			return Iterator{ StringPointer + Length };
		}
		auto end() const {
			return ConstantIterator{ StringPointer + Length };
		}
		auto Size() const {
			return Length;
		}
		auto Data() {
			return StringPointer;
		}
		auto Data() const {
			return CastToConstantPointer(StringPointer);
		}
		auto Substring(std::size_t Position, std::size_t Length = NPOS) const {
			if (Position + Length > this->Length || Length == NPOS)
				Length = this->Length - Position;
			return BasicString{ StringPointer + Position, Length };
		}
		auto Find(const BasicString &StringToBeFound, std::size_t Position = 0) const {
			auto GetMaximum = [](auto x, auto y) {
				return x < y ? y : x;
			};
			auto ShiftTable = reinterpret_cast<std::size_t *>(alloca(GetMaximum(StringToBeFound.Length, 2) * sizeof(std::size_t)));
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
				while (Position < StringToBeFound.Length)
					if (StringToBeFound[Position - 1] == StringToBeFound[Cursor])
						RecordCursorAndMoveOn();
					else if (Cursor > 0)
						RelocateCursor();
					else
						SkipAndMoveOn();
			};
			auto OptimizeShiftTable = [&]() {
				for (auto Cursor = 1_size; Cursor < StringToBeFound.Length; ++Cursor)
					if (StringToBeFound[Cursor] == StringToBeFound[ShiftTable[Cursor]])
						ShiftTable[Cursor] = ShiftTable[ShiftTable[Cursor]];
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
				while (Cursor < StringToBeFound.Length && Position < Length)
					if (Self[Position] == StringToBeFound[Cursor])
						MoveOn();
					else
						RelocateCursor();
				if (Cursor == StringToBeFound.Length)
					return Position - StringToBeFound.Length;
				else
					return NPOS;
			};
			InitializeShiftTable();
			OptimizeShiftTable();
			return ActualFind();
		}
		auto &operator[](std::size_t Position) {
			return StringPointer[Position];
		}
		const auto &operator[](std::size_t Position) const {
			return StringPointer[Position];
		}
		auto &operator+=(const BasicString &SomeBasicString) {
			if (SomeBasicString.Length > 0) {
				auto NewStringPointer = new CharacterType[Length + SomeBasicString.Length + 1];
				std::memcpy(NewStringPointer, StringPointer, Length * sizeof(CharacterType));
				std::memcpy(NewStringPointer + Length, SomeBasicString.StringPointer, SomeBasicString.Length * sizeof(CharacterType));
				Length += SomeBasicString.Length;
				NewStringPointer[Length] = 0;
				delete[] StringPointer;
				StringPointer = NewStringPointer;
			}
			return *this;
		}
		friend auto operator+(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return BasicString{ BasicStringA } += BasicStringB;
		}
		friend auto operator+(BasicString &&BasicStringA, const BasicString &BasicStringB) {
			return BasicStringA += BasicStringB;
		}
		friend auto operator==(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			auto ElementWiseCompare = [&]() {
				for (auto Cursor = 0_size; Cursor < BasicStringA.Length; ++Cursor)
					if (BasicStringA[Cursor] == BasicStringB[Cursor])
						continue;
					else
						return false;
				return true;
			};
			if (BasicStringA.Length != BasicStringB.Length)
				return false;
			else
				return ElementWiseCompare();
		}
		friend auto operator!=(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return !(BasicStringA == BasicStringB);
		}
		friend auto operator<(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return FindTheFirstDifferentPairOfCharactersThenCompare(BasicStringA, BasicStringB, [](auto x, auto y) {
				return x < y;
			});
		}
		friend auto operator>(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return FindTheFirstDifferentPairOfCharactersThenCompare(BasicStringA, BasicStringB, [](auto x, auto y) {
				return x > y;
			});
		}
		friend auto operator<=(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return !(BasicStringA > BasicStringB);
		}
		friend auto operator>=(const BasicString &BasicStringA, const BasicString &BasicStringB) {
			return !(BasicStringA < BasicStringB);
		}
		friend auto &operator<<(std::basic_ostream<CharacterType, std::char_traits<CharacterType>> &Output, const BasicString &SomeBasicString) {
			if (SomeBasicString.StringPointer != nullptr)
				Output << SomeBasicString.StringPointer;
			return Output;
		}
	};

	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
	using U16String = BasicString<char16_t>;
	using U32String = BasicString<char32_t>;

	inline namespace Literals {
		inline namespace StringLiterals {
			auto operator""s(const char *SomeStringPointer, std::size_t Length) {
				return String{ SomeStringPointer, Length };
			}

			auto operator""s(const char16_t *SomeStringPointer, std::size_t Length) {
				return U16String{ SomeStringPointer, Length };
			}

			auto operator""s(const char32_t *SomeStringPointer, std::size_t Length) {
				return U32String{ SomeStringPointer, Length };
			}

			auto operator""s(const wchar_t *SomeStringPointer, std::size_t Length) {
				return WString{ SomeStringPointer, Length };
			}
		}
	}
}
