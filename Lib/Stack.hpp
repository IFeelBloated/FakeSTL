#pragma once
#include "Helpers.hpp"
#include "Deque.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary {
	template<typename GenericType, typename Container = Deque<GenericType>>
	class Stack final {
		Container *ContainerPointer = nullptr;
	public:
		Stack() {
			ContainerPointer = new Container{};
		}
		Stack(std::initializer_list<GenericType> Initialization) {
			ContainerPointer = new Container{ Initialization };
		}
		Stack(Stack &&OtherStack) {
			*this = Move(OtherStack);
		}
		Stack(const Stack &OtherStack) :Stack{} {
			*this = OtherStack;
		}
		auto &operator=(Stack &&OtherStack) {
			if (this != &OtherStack)
				Swap(ContainerPointer, OtherStack.ContainerPointer);
			return *this;
		}
		auto &operator=(const Stack &OtherStack) {
			if (this != &OtherStack)
				*ContainerPointer = *OtherStack.ContainerPointer;
			return *this;
		}
		~Stack() {
			delete ContainerPointer;
		}
		auto &Top() {
			return *--ContainerPointer->end();
		}
		const auto &Top() const {
			return *--ContainerPointer->end();
		}
		auto &operator+=(GenericType &&SomeElement) {
			ContainerPointer->PushBack(Move(SomeElement));
			return *this;
		}
		auto &operator+=(const GenericType &SomeElement) {
			ContainerPointer->PushBack(SomeElement);
			return *this;
		}
		auto &operator--() {
			ContainerPointer->PopBack();
			return *this;
		}
		auto Push(GenericType &&SomeElement) {
			*this += Move(SomeElement);
		}
		auto Push(const GenericType &SomeElement) {
			*this += SomeElement;
		}
		auto Pop() {
			--*this;
		}
		auto Empty() const {
			return ContainerPointer->Empty();
		}
		auto Size() const {
			return ContainerPointer->Size();
		}
	};
}