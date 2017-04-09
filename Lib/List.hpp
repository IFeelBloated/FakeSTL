#pragma once
#include "Helpers.hpp"

namespace StandardTemplateLibrary {
	template<typename T>
	struct ListNode final {
		T *Value = nullptr;
		ListNode *Next = nullptr;
		ListNode *Previous = nullptr;
		ListNode() = default;
		ListNode(const T &Object) {
			Value = new T{ Object };
		}
		ListNode(T &&Object) {
			Value = new T{ static_cast<T &&>(Object) };
		}
		ListNode(const ListNode &) = delete;
		ListNode(ListNode &&) = delete;
		auto &operator=(const ListNode &) = delete;
		auto &operator=(ListNode &&) = delete;
		~ListNode() {
			delete Value;
		}
	};

	template<typename T>
	struct ListIteratorBase {
		ListNode<T> *Pointer = nullptr;
		ListIteratorBase() = default;
		ListIteratorBase(ListNode<T> *Item) {
			Pointer = Item;
		}
		ListIteratorBase(const ListIteratorBase &) = default;
		ListIteratorBase(ListIteratorBase &&) = default;
		auto operator=(const ListIteratorBase &)->ListIteratorBase & = default;
		auto operator=(ListIteratorBase &&)->ListIteratorBase & = default;
		~ListIteratorBase() = default;
		friend auto operator==(ListIteratorBase ObjectA, ListIteratorBase ObjectB) {
			return ObjectA.Pointer == ObjectB.Pointer;
		}
		friend auto operator!=(ListIteratorBase ObjectA, ListIteratorBase ObjectB) {
			return ObjectA.Pointer != ObjectB.Pointer;
		}
	};

	template<typename T>
	struct ListIterator final :public ListIteratorBase<T> {
		using ListIteratorBase::ListIteratorBase;
		ListIterator(const ListIterator &) = default;
		ListIterator(ListIterator &&) = default;
		auto operator=(const ListIterator &)->ListIterator & = default;
		auto operator=(ListIterator &&)->ListIterator & = default;
		~ListIterator() = default;
		auto &operator*() const {
			return *Pointer->Value;
		}
		auto *operator->() const {
			return Pointer->Value;
		}
		auto &operator++() {
			Pointer = Pointer->Next;
			return *this;
		}
		auto &operator--() {
			Pointer = Pointer->Previous;
			return *this;
		}
	};

	template<typename T>
	struct ConstantListIterator final :public ListIteratorBase<T> {
		using ListIteratorBase::ListIteratorBase;
		ConstantListIterator(const ConstantListIterator &) = default;
		ConstantListIterator(ConstantListIterator &&) = default;
		auto operator=(const ConstantListIterator &)->ConstantListIterator & = default;
		auto operator=(ConstantListIterator &&)->ConstantListIterator & = default;
		~ConstantListIterator() = default;
		ConstantListIterator(ListIterator<T> Item) {
			Pointer = Item.Pointer;
		}
		const auto &operator*() const {
			return *Pointer->Value;
		}
		const auto *operator->() const {
			return Pointer->Value;
		}
		auto &operator++() {
			Pointer = Pointer->Next;
			return *this;
		}
		auto &operator--() {
			Pointer = Pointer->Previous;
			return *this;
		}
	};

	template<typename T>
	class List final {
		ListNode<T> *Head = nullptr;
		decltype(0_size) Length = 0;
		auto InsertNode(ConstantListIterator<T> Position, ListNode<T> *NewNode) {
			NewNode->Next = Position.Pointer;
			NewNode->Previous = Position.Pointer->Previous;
			Position.Pointer->Previous->Next = NewNode;
			Position.Pointer->Previous = NewNode;
			++Length;
			return ListIterator<T>{ NewNode };
		}
	public:
		List() {
			Head = new ListNode<T>{};
			Head->Next = Head;
			Head->Previous = Head;
		}
		List(std::initializer_list<T> Initialization) :List{} {
			for (auto &x : Initialization)
				*this += x;
		}
		List(List &&Object) :List{} {
			*this = static_cast<List &&>(Object);
		}
		List(const List &Object) :List{} {
			*this = Object;
		}
		auto &operator=(List &&Object) {
			if (this != &Object) {
				auto TemporaryPointer = Head;
				Head = Object.Head;
				Object.Head = TemporaryPointer;
				Length = Object.Length;
			}
			return *this;
		}
		auto &operator=(const List &Object) {
			if (this != &Object) {
				Clear();
				for (auto &x : Object)
					*this += x;
			}
			return *this;
		}
		~List() {
			Clear();
			delete Head;
		}
		auto PushFront(const T &Object) {
			Insert(begin(), Object);
		}
		auto PushFront(T &&Object) {
			Insert(begin(), static_cast<T &&>(Object));
		}
		auto PushBack(const T &Object) {
			*this += Object;
		}
		auto PushBack(T &&Object) {
			*this += static_cast<T &&>(Object);
		}
		auto &operator+=(const List &Object) {
			*this += List{ Object };
			return *this;
		}
		auto &operator+=(List &&Object) {
			if (Object.Empty() == false) {
				Head->Previous->Next = Object.Head->Next;
				Object.Head->Previous->Next = Head;
				Object.Head->Next = Object.Head;
				Object.Head->Previous = Object.Head;
			}
			return *this;
		}
		auto &operator+=(const T &Object) {
			Insert(end(), Object);
			return *this;
		}
		auto &operator+=(T &&Object) {
			Insert(end(), static_cast<T &&>(Object));
			return *this;
		}
		auto Insert(ConstantListIterator<T> Position, const T &Object) {
			return InsertNode(Position, new ListNode<T>{ Object });
		}
		auto Insert(ConstantListIterator<T> Position, T &&Object) {
			return InsertNode(Position, new ListNode<T>{ static_cast<T &&>(Object) });
		}
		auto PopFront() {
			Erase(begin());
		}
		auto PopBack() {
			--*this;
		}
		auto &operator--() {
			Erase(--end());
			return *this;
		}
		auto Erase(ConstantListIterator<T> Position) {
			auto Pointer = Position.Pointer->Next;
			Position.Pointer->Previous->Next = Position.Pointer->Next;
			Position.Pointer->Next->Previous = Position.Pointer->Previous;
			delete Position.Pointer;
			--Length;
			return ListIterator<T>{ Pointer };
		}
		auto Clear() {
			for (auto i = begin(); i != end(); ++i)
				Erase(i);
		}
		auto begin() {
			return ListIterator<T>{ Head->Next };
		}
		auto begin() const {
			return ConstantListIterator<T>{ Head->Next };
		}
		auto end() {
			return ListIterator<T>{ Head };
		}
		auto end() const {
			return ConstantListIterator<T>{ Head };
		}
		auto Size() const {
			return Length;
		}
		auto Empty() const {
			return Length == 0;
		}
		auto Merge(List &Object) {
			auto TemporaryList = List{};
			auto ShiftFirstNodeToTemporaryList = [&](auto &SourceList) {
				auto FirstNode = SourceList.Head->Next;
				SourceList.Head->Next = FirstNode->Next;
				FirstNode->Next->Previous = SourceList.Head;
				--SourceList.Length;
				TemporaryList.InsertNode(TemporaryList.end(), FirstNode);
			};
			auto MergeInOrder = [&]() {
				while (!this->Empty() && !Object.Empty())
					if (*this->begin() < *Object.begin())
						ShiftFirstNodeToTemporaryList(*this);
					else
						ShiftFirstNodeToTemporaryList(Object);
			};
			auto ListCleanup = [&]() {
				while (!this->Empty())
					ShiftFirstNodeToTemporaryList(*this);
				while (!Object.Empty())
					ShiftFirstNodeToTemporaryList(Object);
			};
			if (this != &Object) {
				MergeInOrder();
				ListCleanup();
				*this = static_cast<List &&>(TemporaryList);
			}
		}
		auto Merge(List &&Object) {
			Merge(Object);
		}
		auto Sort() {
			auto TemporaryListArray = List<List>{};
			auto Initialize = [&]() {
				auto WrapFirstNodeAsList = [this]() {
					auto TemporaryList = List{};
					auto FirstNode = Head->Next;
					Head->Next = FirstNode->Next;
					FirstNode->Next->Previous = Head;
					--Length;
					TemporaryList.InsertNode(TemporaryList.end(), FirstNode);
					return TemporaryList;
				};
				while (!Empty())
					TemporaryListArray += WrapFirstNodeAsList();
			};
			auto MergeSort = [&]() {
				auto EndPosition = TemporaryListArray.Size() % 2 == 0 ? TemporaryListArray.end() : --TemporaryListArray.end();
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				for (auto i = TemporaryListArray.begin(); i != EndPosition; i = TemporaryListArray.Erase(GetNext(i)))
					i->Merge(*GetNext(i));
			};
			if (Length > 1) {
				Initialize();
				while (TemporaryListArray.Size() > 1)
					MergeSort();
				*this = static_cast<List &&>(*TemporaryListArray.begin());
			}
		}
		auto Reverse() {
			auto TemporaryList = List{};
			auto ShiftLastNodeToTemporaryList = [&]() {
				auto LastNode = Head->Previous;
				LastNode->Previous->Next = Head;
				Head->Previous = LastNode->Previous;
				--Length;
				TemporaryList.InsertNode(TemporaryList.end(), LastNode);
			};
			if (Length > 1) {
				while (!Empty())
					ShiftLastNodeToTemporaryList();
				*this = static_cast<List &&>(TemporaryList);
			}
		}
		friend auto operator+(List &&ObjectA, List &&ObjectB) {
			return ObjectA += static_cast<List &&>(ObjectB);
		}
		friend auto operator+(List &&ObjectA, const List &ObjectB) {
			return ObjectA += ObjectB;
		}
		friend auto operator+(const List &ObjectA, List &&ObjectB) {
			return List{ ObjectA } + static_cast<List &&>(ObjectB);
		}
		friend auto operator+(const List &ObjectA, const List &ObjectB) {
			return List{ ObjectA } + List{ ObjectB };
		}
		friend auto operator+(List &&Object, T &&Element) {
			return Object += static_cast<T &&>(Element);
		}
		friend auto operator+(T &&Element, List &&Object) {
			Object.PushFront(static_cast<T &&>(Element));
			return Object;
		}
		friend auto operator+(List &&Object, const T &Element) {
			return Object += Element;
		}
		friend auto operator+(const T &Element, List &&Object) {
			Object.PushFront(Element);
			return Object;
		}
		friend auto operator+(const List &Object, T &&Element) {
			return List{ Object } += static_cast<T &&>(Element);
		}
		friend auto operator+(T &&Element, const List &Object) {
			auto TemporaryObject = List{ Object };
			TemporaryObject.PushFront(static_cast<T &&>(Element));
			return TemporaryObject;
		}
		friend auto operator+(const List &Object, const T &Element) {
			return List{ Object } += Element;
		}
		friend auto operator+(const T &Element, const List &Object) {
			auto TemporaryObject = List{ Object };
			TemporaryObject.PushFront(Element);
			return TemporaryObject;
		}
	};
}
