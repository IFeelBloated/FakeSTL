#pragma once
#include <initializer_list>

namespace StandardTemplateLibrary {
	template<typename T = auto>
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

	template<typename T = auto>
	class ListIterator final {
		ListNode<T> *Pointer = nullptr;
	public:
		ListIterator() = delete;
		ListIterator(ListNode<T> *Item) {
			Pointer = Item;
		}
		ListIterator(const ListIterator &) = default;
		ListIterator(ListIterator &&) = default;
		auto operator=(const ListIterator &)->ListIterator & = default;
		auto operator=(ListIterator &&)->ListIterator & = default;
		~ListIterator() = default;
		auto &operator*() const {
			return *Pointer->Value;
		}
		auto &operator++() {
			Pointer = Pointer->Next;
			return *this;
		}
		auto &operator--() {
			Pointer = Pointer->Previous;
			return *this;
		}
		auto operator==(const ListIterator &Object) const {
			return Pointer == Object.Pointer;
		}
		auto operator!=(const ListIterator &Object) const {
			return Pointer != Object.Pointer;
		}
		auto Get() const {
			return Pointer;
		}
	};

	template<typename T = auto>
	class List final {
		ListNode<T> *Head = nullptr;
		size_t Length = 0;
		auto InsertNode(const ListIterator<T> Position, ListNode<T> *NewNode) {
			NewNode->Next = Position.Get();
			NewNode->Previous = Position.Get()->Previous;
			Position.Get()->Previous->Next = NewNode;
			Position.Get()->Previous = NewNode;
			++Length;
			return ListIterator<T>{ NewNode };
		};
	public:
		List() {
			Head = new ListNode<T>{};
			Head->Next = Head;
			Head->Previous = Head;
		}
		List(std::initializer_list<T> Initialization) :List{} {
			for (auto &x : Initialization)
				PushBack(x);
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
					PushBack(x);
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
			Insert(end(), Object);
		}
		auto PushBack(T &&Object) {
			Insert(end(), static_cast<T &&>(Object));
		}
		auto Insert(const ListIterator<T> Position, const T &Object) {
			return InsertNode(Position, new ListNode<T>{ Object });
		}
		auto Insert(const ListIterator<T> Position, T &&Object) {
			return InsertNode(Position, new ListNode<T>{ static_cast<T &&>(Object) });
		}
		auto PopFront() {
			Erase(begin());
		}
		auto PopBack() {
			Erase(--end());
		}
		auto Erase(const ListIterator<T> Position) {
			auto Pointer = Position.Get()->Next;
			Position.Get()->Previous->Next = Position.Get()->Next;
			Position.Get()->Next->Previous = Position.Get()->Previous;
			delete Position.Get();
			--Length;
			return ListIterator<T>{ Pointer };
		}
		auto Clear() {
			for (auto i = begin(); i != end(); ++i)
				Erase(i);
		}
		auto begin() const {
			return ListIterator<T>{ Head->Next };
		}
		auto end() const {
			return ListIterator<T>{ Head };
		}
		auto Size() const {
			return Length;
		}
		auto Empty() const {
			return Length == 0;
		}
	};
}
