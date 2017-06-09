#include <iostream>
#include "Helpers.hpp"
#include "Utility.hpp"
#include "List.hpp"

namespace StandardTemplateLibrary::Extras {
	template<typename GenericType>
	class HuffmanTree final {
		struct HuffmanTreeNode final {
			GenericType *ElementPointer = nullptr;
			HuffmanTreeNode *LeftChild = nullptr;
			HuffmanTreeNode *RightChild = nullptr;
			decltype(0.) Weight = 0.;
			HuffmanTreeNode() = default;
			HuffmanTreeNode(const GenericType &SomeElement, double Weight) {
				this->ElementPointer = new GenericType{ SomeElement };
				this->Weight = Weight;
			}
			HuffmanTreeNode(GenericType &&SomeElement, double Weight) {
				this->ElementPointer = new GenericType{ static_cast<GenericType &&>(SomeElement) };
				this->Weight = Weight;
			}
			HuffmanTreeNode(HuffmanTreeNode &&OtherHuffmanTreeNode) {
				*this = static_cast<HuffmanTreeNode &&>(OtherHuffmanTreeNode);
			}
			HuffmanTreeNode(const HuffmanTreeNode &OtherHuffmanTreeNode) {
				ElementPointer = new GenericType{};
				LeftChild = new HuffmanTreeNode{};
				RightChild = new HuffmanTreeNode{};
				*this = OtherHuffmanTreeNode;
			}
			auto &operator=(HuffmanTreeNode &&OtherHuffmanTreeNode) {
				if (this != &OtherHuffmanTreeNode) {
					Swap(ElementPointer, OtherHuffmanTreeNode.ElementPointer);
					Swap(LeftChild, OtherHuffmanTreeNode.LeftChild);
					Swap(RightChild, OtherHuffmanTreeNode.RightChild);
					Weight = OtherHuffmanTreeNode.Weight;
				}
				return *this;
			}
			auto operator=(const HuffmanTreeNode &OtherHuffmanTreeNode)->decltype(*this) {
				auto CopyFromThePointerIfPossible = [](auto SourcePointer, auto &DestinationPointer) {
					auto RestoreTheDestinationPointer = [&]() {
						delete DestinationPointer;
						DestinationPointer = nullptr;
					};
					if (SourcePointer != nullptr)
						*DestinationPointer = *SourcePointer;
					else
						RestoreTheDestinationPointer();
				};
				if (this != &OtherHuffmanTreeNode) {
					CopyFromThePointerIfPossible(OtherHuffmanTreeNode.ElementPointer, ElementPointer);
					CopyFromThePointerIfPossible(OtherHuffmanTreeNode.LeftChild, LeftChild);
					CopyFromThePointerIfPossible(OtherHuffmanTreeNode.RightChild, RightChild);
					Weight = OtherHuffmanTreeNode.Weight;
				}
				return *this;
			}
			~HuffmanTreeNode() {
				delete ElementPointer;
				delete LeftChild;
				delete RightChild;
			}
			auto operator<(const HuffmanTreeNode &OtherHuffmanTreeNode) const {
				return Weight < OtherHuffmanTreeNode.Weight;
			}
		};
		HuffmanTreeNode *Root = nullptr;
	public:
		HuffmanTree() {
			Root = new HuffmanTreeNode{};
		}
		HuffmanTree(std::initializer_list<HuffmanTreeNode> Initialization) :HuffmanTree{} {
			using Container = List<HuffmanTreeNode>;
			auto ListOfConstructedNodes = Container{ Initialization };
			auto Unite2NodesOfLowestWeightAsOne = [&]() {
				auto NodeOfLowestWeight = new HuffmanTreeNode{ static_cast<HuffmanTreeNode &&>(*ListOfConstructedNodes.begin()) };
				auto NodeOf2ndLowestWeight = new HuffmanTreeNode{ static_cast<HuffmanTreeNode &&>(*++ListOfConstructedNodes.begin()) };
				auto UnitedNode = HuffmanTreeNode{};
				auto ConstructTheUnitedNode = [&]() {
					UnitedNode.LeftChild = NodeOfLowestWeight;
					UnitedNode.RightChild = NodeOf2ndLowestWeight;
					UnitedNode.Weight = NodeOfLowestWeight->Weight + NodeOf2ndLowestWeight->Weight;
				};
				auto InsertTheUnitedNodeBackToList = [&]() {
					ListOfConstructedNodes.PopFront();
					ListOfConstructedNodes.PopFront();
					auto Cursor = ListOfConstructedNodes.begin();
					while (Cursor != ListOfConstructedNodes.end() && Cursor->Weight < UnitedNode.Weight)
						++Cursor;
					ListOfConstructedNodes.Insert(Cursor, static_cast<HuffmanTreeNode &&>(UnitedNode));
				};
				ConstructTheUnitedNode();
				InsertTheUnitedNodeBackToList();
			};
			ListOfConstructedNodes.Sort();
			while (ListOfConstructedNodes.Size() > 1)
				Unite2NodesOfLowestWeightAsOne();
			*Root = static_cast<HuffmanTreeNode &&>(*ListOfConstructedNodes.begin());
		}
		HuffmanTree(HuffmanTree &&OtherHuffmanTree) {
			*this = static_cast<HuffmanTree &&>(OtherHuffmanTree);
		}
		HuffmanTree(const HuffmanTree &OtherHuffmanTree) :HuffmanTree{} {
			*this = OtherHuffmanTree;
		}
		auto &operator=(HuffmanTree &&OtherHuffmanTree) {
			if (this != &OtherHuffmanTree)
				Swap(Root, OtherHuffmanTree.Root);
			return *this;
		}
		auto &operator=(const HuffmanTree &OtherHuffmanTree) {
			if (this != &OtherHuffmanTree)
				*Root = *OtherHuffmanTree.Root;
			return *this;
		}
		~HuffmanTree() {
			delete Root;
		}
	};
}

auto main()->int {
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	Pause();
}