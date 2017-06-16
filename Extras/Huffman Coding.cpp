#include <iostream>
#include "Helpers.hpp"
#include "Utility.hpp"
#include "List.hpp"
#include "TypeTraits.hpp"

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
				this->ElementPointer = new GenericType{ Move(SomeElement) };
				this->Weight = Weight;
			}
			HuffmanTreeNode(HuffmanTreeNode &&OtherHuffmanTreeNode) {
				*this = Move(OtherHuffmanTreeNode);
			}
			HuffmanTreeNode(const HuffmanTreeNode &OtherHuffmanTreeNode) {
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
			auto IsLeaf() {
				return ElementPointer != nullptr;
			}
			auto operator<(const HuffmanTreeNode &OtherHuffmanTreeNode) const {
				return Weight < OtherHuffmanTreeNode.Weight;
			}
		};
		struct CodingUnit final {
			GenericType Element = {};
			decltype(0_uint64) Code = 0;
			static constexpr auto AnchorBit = 1_uint64;
			CodingUnit() = default;
			CodingUnit(GenericType &&SomeElement, std::uint64_t SomeCode) {
				Element = Move(SomeElement);
				Code = SomeCode;
			}
			CodingUnit(const GenericType &SomeElement, std::uint64_t SomeCode) {
				Element = SomeElement;
				Code = SomeCode;
			}
			CodingUnit(CodingUnit &&OtherCodingUnit) {
				*this = Move(OtherCodingUnit);
			}
			CodingUnit(const CodingUnit &) = default;
			auto &operator=(CodingUnit &&OtherCodingUnit) {
				if (this != &OtherCodingUnit) {
					Element = Move(OtherCodingUnit.Element);
					Code = OtherCodingUnit.Code;
				}
				return *this;
			}
			auto operator=(const CodingUnit &)->decltype(*this) = default;
			~CodingUnit() = default;
			friend auto &operator<<(std::ostream &Output, const CodingUnit &SomeCodingUnit) {
				constexpr auto AnchorBit = CodingUnit::AnchorBit;
				auto PrintBinaryCode = [&](auto Code) {
					auto BitContainer = List<decltype(Code)>{};
					if (Code == AnchorBit)
						Output << 0;
					while (Code != AnchorBit) {
						BitContainer += Code & 0x1;
						Code >>= 1;
					}
					BitContainer.Reverse();
					for (auto x : BitContainer)
						Output << x;
				};
				Output << SomeCodingUnit.Element << ": ";
				PrintBinaryCode(SomeCodingUnit.Code);
				return Output;
			}
		};
		using CodingUnitContainer = List<CodingUnit>;
		HuffmanTreeNode *Root = nullptr;
		auto TagEachNodeRecursively(HuffmanTreeNode *NodeCursor, std::uint64_t ConstructedCode, CodingUnitContainer &CodingResult)->void {
			auto CompleteTagging = [&]() {
				CodingResult += { Move(*NodeCursor->ElementPointer), ConstructedCode };
			};
			auto KeepOnTagging = [&]() {
				auto TagLeftChild = [](auto PartialCode) {
					return PartialCode << 1;
				};
				auto TagRightChild = [](auto PartialCode) {
					PartialCode <<= 1;
					return PartialCode + 1;
				};
				TagEachNodeRecursively(NodeCursor->LeftChild, TagLeftChild(ConstructedCode), CodingResult);
				TagEachNodeRecursively(NodeCursor->RightChild, TagRightChild(ConstructedCode), CodingResult);
			};
			if (NodeCursor->IsLeaf())
				CompleteTagging();
			else
				KeepOnTagging();
		}
	public:
		HuffmanTree() {
			Root = new HuffmanTreeNode{};
		}
		HuffmanTree(std::initializer_list<HuffmanTreeNode> Initialization) :HuffmanTree{} {
			using Container = List<HuffmanTreeNode>;
			auto ListOfConstructedNodes = Container{ Initialization };
			auto Unite2NodesOfLowestWeightAsOne = [&]() {
				auto NodeOfLowestWeight = new HuffmanTreeNode{ Move(*ListOfConstructedNodes.begin()) };
				auto NodeOf2ndLowestWeight = new HuffmanTreeNode{ Move(*++ListOfConstructedNodes.begin()) };
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
					ListOfConstructedNodes.Insert(Cursor, Move(UnitedNode));
				};
				ConstructTheUnitedNode();
				InsertTheUnitedNodeBackToList();
			};
			ListOfConstructedNodes.Sort();
			while (ListOfConstructedNodes.Size() > 1)
				Unite2NodesOfLowestWeightAsOne();
			*Root = Move(*ListOfConstructedNodes.begin());
		}
		HuffmanTree(HuffmanTree &&OtherHuffmanTree) {
			*this = Move(OtherHuffmanTree);
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
		auto Encode() {
			auto CodingResult = List<CodingUnit>{};
			TagEachNodeRecursively(Root, CodingUnit::AnchorBit, CodingResult);
			return CodingResult;
		}
	};
}

auto main()->int {
	using SetOfWeightedLetters = StandardTemplateLibrary::Extras::HuffmanTree<decltype('a')>;
	auto TestSet = SetOfWeightedLetters{ { 'c', 10 }, { 'a', 3 }, { 'e', 8 }, { 'd', 2 }, { 'f', 20 }, { 'k', 11 } };
	auto CodingResult = TestSet.Encode();
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "Huffman Coding Result:" << std::endl;
	for (auto &x : CodingResult)
		std::cout << x << std::endl;
	Pause();
}