#pragma once
#include "Helpers.hpp"
#include "Deque.hpp"

namespace StandardTemplateLibrary {
	template<typename IteratorType>
	class StackIterator final {
		mutable IteratorType Iterator;
	public:
		StackIterator() = delete;
		StackIterator(IteratorType Item) :Iterator{ Item } {}
		StackIterator(StackIterator &&) = default;
		StackIterator(const StackIterator &) = default;
		auto operator=(StackIterator &&)->StackIterator & = default;
		auto operator=(const StackIterator &)->StackIterator & = default;
		~StackIterator() = default;
		auto &operator*() {
			return *Iterator;
		}
		const auto &operator*() const {
			return *Iterator;
		}
		auto operator->() {
			return Iterator;
		}
		const auto operator->() const {
			return Iterator;
		}
		auto &operator++() const {
			--Iterator;
			return *this;
		}
		auto &operator--() const {
			++Iterator;
			return *this;
		}
		auto operator==(const StackIterator &Object) const {
			return Iterator == Object.Iterator;
		}
		auto operator!=(const StackIterator &Object) const {
			return Iterator != Object.Iterator;
		}
	};

	template<typename T, typename Container = Deque<T>>
	class Stack final {
		Container *Pointer = nullptr;
	public:
		Stack() {
			Pointer = new Container{};
		}
		Stack(std::initializer_list<T> Initialization) {
			Pointer = new Container{ Initialization };
		}
		Stack(Stack &&Object) {
			*this = static_cast<Stack &&>(Object);
		}
		Stack(const Stack &Object) :Stack{} {
			*this = Object;
		}
		auto &operator=(Stack &&Object) {
			if (this != &Object) {
				auto TemporaryPointer = Pointer;
				Pointer = Object.Pointer;
				Object.Pointer = TemporaryPointer;
			}
			return *this;
		}
		auto &operator=(const Stack &Object) {
			if (this != &Object)
				*Pointer = *Object.Pointer;
			return *this;
		}
		~Stack() {
			delete Pointer;
		}
		auto &Top() {
			return *--Pointer->end();
		}
		const auto &Top() const {
			return *--Pointer->end();
		}
		auto &operator+=(T &&Object) {
			Pointer->PushBack(static_cast<T &&>(Object));
			return *this;
		}
		auto &operator+=(const T &Object) {
			Pointer->PushBack(Object);
			return *this;
		}
		auto &operator--() {
			Pointer->PopBack();
			return *this;
		}
		auto Push(T &&Object) {
			*this += static_cast<T &&>(Object);
		}
		auto Push(const T &Object) {
			*this += Object;
		}
		auto Pop() {
			--*this;
		}
		auto Empty() const {
			return Pointer->Empty();
		}
		auto Size() const {
			return Pointer->Size();
		}
		auto begin() {
			return StackIterator<decltype(Pointer->begin())>{--Pointer->end()};
		}
		const auto begin() const {
			return StackIterator<decltype(Pointer->begin())>{--Pointer->end()};
		}
		auto end() {
			return StackIterator<decltype(Pointer->end())>{--Pointer->begin()};
		}
		const auto end() const {
			return StackIterator<decltype(Pointer->end())>{--Pointer->begin()};
		}
	};
}