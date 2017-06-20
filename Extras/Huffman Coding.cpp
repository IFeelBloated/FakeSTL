#include <iostream>
#include "Helpers.hpp"
#include "Utility.hpp"
#include "List.hpp"
#include "TypeTraits.hpp"
#include "MacroHacks.hpp"

namespace StandardTemplateLibrary::Extras {
	template<typename GenericType>
	class HuffmanTree final {
		struct HuffmanTreeNode final {
			self(ElementPointer, static_cast<GenericType *>(nullptr));
			self(LeftChild, static_cast<HuffmanTreeNode *>(nullptr));
			self(RightChild, static_cast<HuffmanTreeNode *>(nullptr));
			self(Weight, 0.);
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
			self(Element, GenericType{});
			self(Code, 0ull);
			CodingUnit() = default;
			CodingUnit(GenericType &&SomeElement, unsigned long long SomeCode) {
				Element = Move(SomeElement);
				Code = SomeCode;
			}
			CodingUnit(const GenericType &SomeElement, unsigned long long SomeCode) {
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
		};
		using CodingUnitContainer = List<CodingUnit>;
		self(Root, static_cast<HuffmanTreeNode *>(nullptr));
		auto TagEachNodeRecursively(HuffmanTreeNode *NodeCursor, unsigned long long ConstructedCode, CodingUnitContainer &CodingResult)->void {
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
		static constexpr auto AnchorBit = 1ull;
		HuffmanTree() {
			Root = new HuffmanTreeNode{};
		}
		HuffmanTree(std::initializer_list<HuffmanTreeNode> Initialization) :HuffmanTree{} {
			using Container = List<HuffmanTreeNode>;
			auto ListOfConstructedNodes = Container{ Initialization };
			auto Unite2NodesOfLowestWeightAsOne = [&]() {
				auto NodeOfLowestWeight = new HuffmanTreeNode{ Move(*ListOfConstructedNodes.Begin()) };
				auto NodeOf2ndLowestWeight = new HuffmanTreeNode{ Move(*++ListOfConstructedNodes.Begin()) };
				auto UnitedNode = HuffmanTreeNode{};
				auto ConstructTheUnitedNode = [&]() {
					UnitedNode.LeftChild = NodeOfLowestWeight;
					UnitedNode.RightChild = NodeOf2ndLowestWeight;
					UnitedNode.Weight = NodeOfLowestWeight->Weight + NodeOf2ndLowestWeight->Weight;
				};
				auto InsertTheUnitedNodeBackToList = [&]() {
					ListOfConstructedNodes.PopFront();
					ListOfConstructedNodes.PopFront();
					auto Cursor = ListOfConstructedNodes.Begin();
					while (Cursor != ListOfConstructedNodes.End() && Cursor->Weight < UnitedNode.Weight)
						++Cursor;
					ListOfConstructedNodes.Insert(Cursor, Move(UnitedNode));
				};
				ConstructTheUnitedNode();
				InsertTheUnitedNodeBackToList();
			};
			ListOfConstructedNodes.Sort();
			while (ListOfConstructedNodes.Size() > 1)
				Unite2NodesOfLowestWeightAsOne();
			*Root = Move(*ListOfConstructedNodes.Begin());
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
			TagEachNodeRecursively(Root, AnchorBit, CodingResult);
			return CodingResult;
		}
	};
}

auto main()->int {
	using SetOfWeightedCharacters = StandardTemplateLibrary::Extras::HuffmanTree<decltype('a')>;
	auto TestSet = SetOfWeightedCharacters{ { 'c', 10 }, { 'a', 3 }, { 'e', 8 }, { 'd', 2 }, { 'f', 20 }, { 'k', 11 } };
	auto CodingResult = TestSet.Encode();
	auto PrintBinaryCode = [](auto Code) {
		using BitContainer = StandardTemplateLibrary::List<decltype(Code)>;
		auto BitBuffer = BitContainer{};
		if (Code == SetOfWeightedCharacters::AnchorBit)
			std::cout << 0;
		while (Code != SetOfWeightedCharacters::AnchorBit) {
			BitBuffer += Code & 0x1;
			Code >>= 1;
		}
		BitBuffer.Reverse();
		for (auto x : BitBuffer)
			std::cout << x;
	};
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "Huffman Coding Result:" << std::endl;
	for (auto &[Character, Code] : CodingResult) {
		std::cout << Character << ": ";
		PrintBinaryCode(Code);
		std::cout << std::endl;
	}
	Pause();
}