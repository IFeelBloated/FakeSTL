#pragma once
#include "Helpers.hpp"

namespace StandardTemplateLibrary {
	template<typename T>
	struct DequeChunk final {
		static constexpr auto MaxLength = 256_size;
		T *Array = nullptr;
		decltype(0_size) Capacity = MaxLength;
		DequeChunk() {
			Array = new T[MaxLength];
		}
		DequeChunk(const DequeChunk &) = delete;
		DequeChunk(DequeChunk &&) = delete;
		auto &operator=(const DequeChunk &) = delete;
		auto &operator=(DequeChunk &&) = delete;
		~DequeChunk() {
			delete[] Array;
		}
		auto &operator+=(const T &Object) {
			Array[MaxLength - Capacity] = Object;
			--Capacity;
			return *this;
		}
		auto &operator+=(T &&Object) {
			Array[MaxLength - Capacity] = static_cast<T &&>(Object);
			--Capacity;
			return *this;
		}
		auto &operator[](std::size_t Position) {
			return Array[Position];
		}
		const auto &operator[](std::size_t Position) const {
			return Array[Position];
		}
	};

	template<typename T>
	class Deque final {
		DequeChunk<T> **Pointer = nullptr;
		decltype(0_size) ChunkCount = 0;
	public:
		Deque() = default;




		~Deque() {
			auto RemoveAllChunks = [this]() {
				for (auto i = 0_size; i < ChunkCount; ++i)
					delete Pointer[i];
			};
			RemoveAllChunks();
			delete[] Pointer;
		}
	};
}