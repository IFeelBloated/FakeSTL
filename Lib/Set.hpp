#pragma once
#include "Helpers.hpp"
#include "Utility.hpp"
#include "MacroHacks.hpp"

namespace StandardTemplateLibrary {
	template<typename GenericType>
	class Set final {
		struct SetNode final {
			self(ElementPointer, static_cast<GenericType *>(nullptr));
			self(Parent, static_cast<SetNode *>(nullptr));
			self(LeftChild, static_cast<SetNode *>(nullptr));
			self(RightChild, static_cast<SetNode *>(nullptr));
			SetNode() = default;
			SetNode(const GenericType &SomeElement) {
				ElementPointer = new GenericType{ SomeElement };
			}
			SetNode(GenericType &&SomeElement) {
				ElementPointer = new GenericType{ Move(SomeElement) };
			}
			SetNode(SetNode &&OtherSetNode) {
				*this = Move(OtherSetNode);
			}
			SetNode(const SetNode &OtherSetNode) {
				*this = OtherSetNode;
			}
			auto &operator=(SetNode &&OtherSetNode) {
				if (this != &OtherSetNode) {
					Swap(ElementPointer, OtherSetNode.ElementPointer);
					Swap(LeftChild, OtherSetNode.LeftChild);
					Swap(RightChild, OtherSetNode.RightChild);
					RecalibrateParentNodeForEachChild();
				}
				return *this;
			}
			auto operator=(const SetNode &OtherSetNode)->decltype(*this) {
				auto CopyFromThePointerIfPossible = [](auto SourcePointer, auto &DestinationPointer) {
					auto InitializeDestinationPointer = [&]() {
						DestinationPointer = new RemoveReference_t<decltype(*DestinationPointer)>{};
					};
					auto ResetDestinationPointer = [&]() {
						delete DestinationPointer;
						DestinationPointer = nullptr;
					};
					if (DestinationPointer == nullptr)
						InitializeDestinationPointer();
					if (SourcePointer != nullptr)
						*DestinationPointer = *SourcePointer;
					else
						ResetDestinationPointer();
				};
				if (this != &OtherSetNode) {
					CopyFromThePointerIfPossible(OtherSetNode.ElementPointer, ElementPointer);
					CopyFromThePointerIfPossible(OtherSetNode.LeftChild, LeftChild);
					CopyFromThePointerIfPossible(OtherSetNode.RightChild, RightChild);
					RecalibrateParentNodeForEachChild();
				}
				return *this;
			}
			~SetNode() {
				delete ElementPointer;
				delete LeftChild;
				delete RightChild;
			}
		private:
			auto RecalibrateParentNodeForEachChild() {
				if (LeftChild != nullptr)
					LeftChild->Parent = this;
				if (RightChild != nullptr)
					RightChild->Parent = this;
			}
		};
		struct ConstantIterator final {
			self(NodePointer, static_cast<SetNode *>(nullptr));
			ConstantIterator() = default;
			ConstantIterator(SetNode *SomeSetNode) {
				NodePointer = SomeSetNode;
			}
			ConstantIterator(ConstantIterator &&) = default;
			ConstantIterator(const ConstantIterator &) = default;
			auto operator=(ConstantIterator &&)->decltype(*this) = default;
			auto operator=(const ConstantIterator &)->decltype(*this) = default;
			~ConstantIterator() = default;
			const auto &operator*() const {
				return *NodePointer->ElementPointer;
			}
			auto operator->() const {
				return CastToConstantPointer(NodePointer->ElementPointer);
			}
			auto &operator++() {
				auto Cursor = NodePointer->RightChild;
				auto GoToTheLeftmostNode = [&]() {
					while (Cursor->LeftChild != nullptr)
						Cursor = Cursor->LeftChild;
				};
				auto RegressToTheNearestAncestorThatHasTheCurrentNodeAsALeftDescendant = [&]() {
					auto TrackingReference = NodePointer;
					auto KeepRegressing = [&]() {
						TrackingReference = Cursor;
						Cursor = Cursor->Parent;
					};
					Cursor = NodePointer->Parent;
					while (Cursor != nullptr && TrackingReference == Cursor->RightChild)
						KeepRegressing();
				};
				if (Cursor != nullptr)
					GoToTheLeftmostNode();
				else
					RegressToTheNearestAncestorThatHasTheCurrentNodeAsALeftDescendant();
				NodePointer = Cursor;
				return *this;
			}
			auto &operator--() {
				auto Cursor = NodePointer->LeftChild;
				auto GoToTheRightmostNode = [&]() {
					while (Cursor->RightChild != nullptr)
						Cursor = Cursor->RightChild;
				};
				auto RegressToTheNearestAncestorThatHasTheCurrentNodeAsARightDescendant = [&]() {
					auto TrackingReference = NodePointer;
					auto KeepRegressing = [&]() {
						TrackingReference = Cursor;
						Cursor = Cursor->Parent;
					};
					Cursor = NodePointer->Parent;
					while (Cursor != nullptr && TrackingReference == Cursor->LeftChild)
						KeepRegressing();
				};
				if (Cursor != nullptr)
					GoToTheRightmostNode();
				else
					RegressToTheNearestAncestorThatHasTheCurrentNodeAsARightDescendant();
				NodePointer = Cursor;
				return *this;
			}
			auto operator==(ConstantIterator OtherConstantIterator) const {
				return NodePointer == OtherConstantIterator.NodePointer;
			}
			auto operator!=(ConstantIterator OtherConstantIterator) const {
				return NodePointer != OtherConstantIterator.NodePointer;
			}
		};
		self(Head, static_cast<SetNode *>(nullptr));
		self(ElementCount, 0_size);
		auto InsertConstructedNode(SetNode *ConstructedNode) {
			auto InsertRootNode = [&]() {
				Head->LeftChild = ConstructedNode;
				ConstructedNode->Parent = Head;
			};
			auto InsertRegularNode = [&]() {
				auto InsertingPosition = Head;
				auto LocateInsertingPosition = [&]() {
					auto Cursor = Head->LeftChild;
					auto HeadToTheLeftChild = [&]() {
						InsertingPosition = Cursor;
						Cursor = Cursor->LeftChild;
					};
					auto HeadToTheRightChild = [&]() {
						InsertingPosition = Cursor;
						Cursor = Cursor->RightChild;
					};
					while (Cursor != nullptr)
						if (*ConstructedNode->ElementPointer < *Cursor->ElementPointer)
							HeadToTheLeftChild();
						else
							HeadToTheRightChild();
				};
				auto InsertTheNodeNow = [&]() {
					if (*ConstructedNode->ElementPointer < *InsertingPosition->ElementPointer)
						InsertingPosition->LeftChild = ConstructedNode;
					else
						InsertingPosition->RightChild = ConstructedNode;
					ConstructedNode->Parent = InsertingPosition;
				};
				LocateInsertingPosition();
				InsertTheNodeNow();
			};
			if (Empty() == true)
				InsertRootNode();
			else
				InsertRegularNode();
			++ElementCount;
		}
	public:
		Set() {
			Head = new SetNode{};
		}
		Set(std::initializer_list<GenericType> Initialization) :Set{} {
			for (auto &x : Initialization)
				*this += x;
		}
		Set(Set &&OtherSet) {
			*this = Move(OtherSet);
		}
		Set(const Set &OtherSet) :Set{} {
			*this = OtherSet;
		}
		auto &operator=(Set &&OtherSet) {
			if (this != &OtherSet) {
				Swap(Head, OtherSet.Head);
				ElementCount = OtherSet.ElementCount;
			}
			return *this;
		}
		auto &operator=(const Set &OtherSet) {
			if (this != &OtherSet) {
				*Head = *OtherSet.Head;
				ElementCount = OtherSet.ElementCount;
			}
			return *this;
		}
		~Set() {
			delete Head;
		}
		auto &operator+=(const GenericType &SomeElement) {
			InsertConstructedNode(new SetNode{ SomeElement });
			return *this;
		}
		auto &operator+=(GenericType &&SomeElement) {
			InsertConstructedNode(new SetNode{ Move(SomeElement) });
			return *this;
		}
		auto Find(const GenericType &ElementToBeFound) const {
			auto Cursor = Head->LeftChild;
			while (Cursor != nullptr)
				if (ElementToBeFound == *Cursor->ElementPointer)
					return ConstantIterator{ Cursor };
				else if (ElementToBeFound < *Cursor->ElementPointer)
					Cursor = Cursor->LeftChild;
				else
					Cursor = Cursor->RightChild;
			return end();
		}
		auto Erase(ConstantIterator Position) {
			auto GetNext = [](auto Iterator) {
				return ++Iterator;
			};
			auto PositionNextToTheErasedPosition = GetNext(Position);
			auto NodeToBeErased = Position.NodePointer;
			auto ParentNode = NodeToBeErased->Parent;
			auto NodeHasNoChildren = [&]() {
				return NodeToBeErased->LeftChild == nullptr && NodeToBeErased->RightChild == nullptr;
			};
			auto NodeHasTwoChildren = [&]() {
				return NodeToBeErased->LeftChild != nullptr && NodeToBeErased->RightChild != nullptr;
			};
			auto EraseNodeWithNoChildren = [&]() {
				if (ParentNode->LeftChild == NodeToBeErased)
					ParentNode->LeftChild = nullptr;
				else
					ParentNode->RightChild = nullptr;
				delete NodeToBeErased;
			};
			auto EraseNodeWithOneChild = [&]() {
				auto ChildNode = NodeToBeErased->LeftChild;
				if (ChildNode == nullptr)
					ChildNode = NodeToBeErased->RightChild;
				if (ParentNode->LeftChild == NodeToBeErased)
					ParentNode->LeftChild = ChildNode;
				else
					ParentNode->RightChild = ChildNode;
				ChildNode->Parent = ParentNode;
				NodeToBeErased->LeftChild = nullptr;
				NodeToBeErased->RightChild = nullptr;
				delete NodeToBeErased;
			};
			auto EraseNodeWithTwoChildren = [&]() {
				auto ReferenceNode = PositionNextToTheErasedPosition.NodePointer;
				auto ReferenceChildNode = ReferenceNode->RightChild;
				auto ReferenceParentNode = ReferenceNode->Parent;
				Swap(NodeToBeErased->ElementPointer, ReferenceNode->ElementPointer);
				PositionNextToTheErasedPosition = Position;
				if (ReferenceParentNode->LeftChild == ReferenceNode)
					ReferenceParentNode->LeftChild = ReferenceChildNode;
				else
					ReferenceParentNode->RightChild = ReferenceChildNode;
				if (ReferenceChildNode != nullptr)
					ReferenceChildNode->Parent = ReferenceParentNode;
				ReferenceNode->RightChild = nullptr;
				delete ReferenceNode;
			};
			if (NodeHasNoChildren() == true)
				EraseNodeWithNoChildren();
			else if (NodeHasTwoChildren() == true)
				EraseNodeWithTwoChildren();
			else
				EraseNodeWithOneChild();
			--ElementCount;
			return PositionNextToTheErasedPosition;
		}
		auto Clear() {
			this->~Set();
			new(this) Set{};
		}
		auto begin() const {
			auto GoToTheLeftmostNode = [this]() {
				auto Cursor = Head;
				while (Cursor->LeftChild != nullptr)
					Cursor = Cursor->LeftChild;
				return Cursor;
			};
			return ConstantIterator{ GoToTheLeftmostNode() };
		}
		auto end() const {
			return ConstantIterator{ Head };
		}
		auto Size() const {
			return ElementCount;
		}
		auto Empty() const {
			return ElementCount == 0;
		}
	};
}