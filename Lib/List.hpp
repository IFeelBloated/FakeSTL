#pragma once
#include "Helpers.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary {
	template<typename GenericType>
	class List final {
		struct ListNode final {
			GenericType *ElementPointer = nullptr;
			ListNode *Next = nullptr;
			ListNode *Previous = nullptr;
			ListNode() = default;
			ListNode(const GenericType &SomeElement) {
				ElementPointer = new GenericType{ SomeElement };
			}
			ListNode(GenericType &&SomeElement) {
				ElementPointer = new GenericType{ static_cast<GenericType &&>(SomeElement) };
			}
			ListNode(const ListNode &) = delete;
			ListNode(ListNode &&) = delete;
			auto &operator=(const ListNode &) = delete;
			auto &operator=(ListNode &&) = delete;
			~ListNode() {
				delete ElementPointer;
			}
		};
		struct IteratorBase {
			ListNode *NodePointer = nullptr;
			IteratorBase() = default;
			IteratorBase(ListNode *SomeListNode) {
				NodePointer = SomeListNode;
			}
			IteratorBase(const IteratorBase &) = default;
			IteratorBase(IteratorBase &&) = default;
			auto operator=(const IteratorBase &)->decltype(*this) = default;
			auto operator=(IteratorBase &&)->decltype(*this) = default;
			~IteratorBase() = default;
			friend auto operator==(IteratorBase IteratorA, IteratorBase IteratorB) {
				return IteratorA.NodePointer == IteratorB.NodePointer;
			}
			friend auto operator!=(IteratorBase IteratorA, IteratorBase IteratorB) {
				return IteratorA.NodePointer != IteratorB.NodePointer;
			}
		};
		struct Iterator final :public IteratorBase {
			using IteratorBase::IteratorBase;
			Iterator(const Iterator &) = default;
			Iterator(Iterator &&) = default;
			auto operator=(const Iterator &)->decltype(*this) = default;
			auto operator=(Iterator &&)->decltype(*this) = default;
			~Iterator() = default;
			auto &operator*() const {
				return *this->NodePointer->ElementPointer;
			}
			auto *operator->() const {
				return this->NodePointer->ElementPointer;
			}
			auto &operator++() {
				this->NodePointer = this->NodePointer->Next;
				return *this;
			}
			auto &operator--() {
				this->NodePointer = this->NodePointer->Previous;
				return *this;
			}
		};
		struct ConstantIterator final :public IteratorBase {
			using IteratorBase::IteratorBase;
			ConstantIterator(Iterator ModifiableIterator) {
				this->NodePointer = ModifiableIterator.NodePointer;
			}
			ConstantIterator(const ConstantIterator &) = default;
			ConstantIterator(ConstantIterator &&) = default;
			auto operator=(const ConstantIterator &)->decltype(*this) = default;
			auto operator=(ConstantIterator &&)->decltype(*this) = default;
			~ConstantIterator() = default;
			const auto &operator*() const {
				return *this->NodePointer->ElementPointer;
			}
			const auto *operator->() const {
				return this->NodePointer->ElementPointer;
			}
			auto &operator++() {
				this->NodePointer = this->NodePointer->Next;
				return *this;
			}
			auto &operator--() {
				this->NodePointer = this->NodePointer->Previous;
				return *this;
			}
		};
		ListNode *Head = nullptr;
		decltype(0_size) Length = 0;
		auto InsertConstructedNode(ConstantIterator Position, ListNode *ConstructedNode) {
			ConstructedNode->Next = Position.NodePointer;
			ConstructedNode->Previous = Position.NodePointer->Previous;
			Position.NodePointer->Previous->Next = ConstructedNode;
			Position.NodePointer->Previous = ConstructedNode;
			++Length;
			return Iterator{ ConstructedNode };
		}
	public:
		List() {
			Head = new ListNode{};
			Head->Next = Head;
			Head->Previous = Head;
		}
		List(std::initializer_list<GenericType> Initialization) :List{} {
			for (auto &x : Initialization)
				*this += x;
		}
		List(List &&OtherList) :List{} {
			*this = static_cast<List &&>(OtherList);
		}
		List(const List &OtherList) :List{} {
			*this = OtherList;
		}
		auto &operator=(List &&OtherList) {
			if (this != &OtherList) {
				Swap(Head, OtherList.Head);
				Length = OtherList.Length;
			}
			return *this;
		}
		auto &operator=(const List &OtherList) {
			if (this != &OtherList) {
				Clear();
				for (auto &x : OtherList)
					*this += x;
			}
			return *this;
		}
		~List() {
			Clear();
			delete Head;
		}
		auto PushFront(const GenericType &SomeElement) {
			Insert(begin(), SomeElement);
		}
		auto PushFront(GenericType &&SomeElement) {
			Insert(begin(), static_cast<GenericType &&>(SomeElement));
		}
		auto PushBack(const GenericType &SomeElement) {
			*this += SomeElement;
		}
		auto PushBack(GenericType &&SomeElement) {
			*this += static_cast<GenericType &&>(SomeElement);
		}
		auto &operator+=(const GenericType &SomeElement) {
			Insert(end(), SomeElement);
			return *this;
		}
		auto &operator+=(GenericType &&SomeElement) {
			Insert(end(), static_cast<GenericType &&>(SomeElement));
			return *this;
		}
		auto Insert(ConstantIterator Position, const GenericType &SomeElement) {
			return InsertConstructedNode(Position, new ListNode{ SomeElement });
		}
		auto Insert(ConstantIterator Position, GenericType &&SomeElement) {
			return InsertConstructedNode(Position, new ListNode{ static_cast<GenericType &&>(SomeElement) });
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
		auto Erase(ConstantIterator Position) {
			auto NodeNextToTheErasedNode = Position.NodePointer->Next;
			Position.NodePointer->Previous->Next = Position.NodePointer->Next;
			Position.NodePointer->Next->Previous = Position.NodePointer->Previous;
			delete Position.NodePointer;
			--Length;
			return Iterator{ NodeNextToTheErasedNode };
		}
		auto Clear() {
			auto Cursor = begin();
			while (Cursor != end())
				Cursor = Erase(Cursor);
		}
		auto begin() {
			return Iterator{ Head->Next };
		}
		auto begin() const {
			return ConstantIterator{ Head->Next };
		}
		auto end() {
			return Iterator{ Head };
		}
		auto end() const {
			return ConstantIterator{ Head };
		}
		auto Size() const {
			return Length;
		}
		auto Empty() const {
			return Length == 0;
		}
		auto Merge(List &OtherList) {
			auto TemporaryList = List{};
			auto ShiftTheFirstNodeToTemporaryList = [&](auto &SourceList) {
				auto FirstNode = SourceList.Head->Next;
				SourceList.Head->Next = FirstNode->Next;
				FirstNode->Next->Previous = SourceList.Head;
				--SourceList.Length;
				TemporaryList.InsertConstructedNode(TemporaryList.end(), FirstNode);
			};
			auto MergeInOrder = [&]() {
				while (!this->Empty() && !OtherList.Empty())
					if (*this->begin() < *OtherList.begin())
						ShiftTheFirstNodeToTemporaryList(*this);
					else
						ShiftTheFirstNodeToTemporaryList(OtherList);
			};
			auto ListCleanup = [&]() {
				while (!this->Empty())
					ShiftTheFirstNodeToTemporaryList(*this);
				while (!OtherList.Empty())
					ShiftTheFirstNodeToTemporaryList(OtherList);
			};
			if (this != &OtherList) {
				MergeInOrder();
				ListCleanup();
				*this = static_cast<List &&>(TemporaryList);
			}
		}
		auto Merge(List &&OtherList) {
			Merge(OtherList);
		}
		auto Sort() {
			auto ListOfPulverizedLists = List<List>{};
			auto Initialize = [&]() {
				auto WrapTheFirstNodeAsList = [this]() {
					auto TemporaryList = List{};
					auto FirstNode = Head->Next;
					Head->Next = FirstNode->Next;
					FirstNode->Next->Previous = Head;
					--Length;
					TemporaryList.InsertConstructedNode(TemporaryList.end(), FirstNode);
					return TemporaryList;
				};
				while (!Empty())
					ListOfPulverizedLists += WrapTheFirstNodeAsList();
			};
			auto MergeSort = [&]() {
				auto Cursor = ListOfPulverizedLists.begin();
				auto EndPosition = ListOfPulverizedLists.Size() % 2 == 0 ? ListOfPulverizedLists.end() : --ListOfPulverizedLists.end();
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				while (Cursor != EndPosition) {
					auto MergingPosition = GetNext(Cursor);
					Cursor->Merge(*MergingPosition);
					Cursor = ListOfPulverizedLists.Erase(MergingPosition);
				}
			};
			if (Length > 1) {
				Initialize();
				while (ListOfPulverizedLists.Size() > 1)
					MergeSort();
				*this = static_cast<List &&>(*ListOfPulverizedLists.begin());
			}
		}
		auto Reverse() {
			auto TemporaryList = List{};
			auto ShiftTheLastNodeToTemporaryList = [&]() {
				auto LastNode = Head->Previous;
				LastNode->Previous->Next = Head;
				Head->Previous = LastNode->Previous;
				--Length;
				TemporaryList.InsertConstructedNode(TemporaryList.end(), LastNode);
			};
			if (Length > 1) {
				while (!Empty())
					ShiftTheLastNodeToTemporaryList();
				*this = static_cast<List &&>(TemporaryList);
			}
		}
	};
}
