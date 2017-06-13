#pragma once
#include "Helpers.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary {
	template<typename RandomAccessIterator>
	auto Sort(RandomAccessIterator Begin, RandomAccessIterator End, std::size_t RecursionDepth = 0)->void {
		static auto InitialBegin = Begin;
		static auto InitialEnd = End;
		static auto MaximumRecursionDepth = static_cast<decltype(0_size)>(2. * std::log2(End - Begin));
		static auto Heapsort = [&]() {
			auto GetLeftChildPosition = [&](auto Position) {
				auto RelativePosition = Position - InitialBegin;
				RelativePosition <<= 1;
				RelativePosition += 1;
				return InitialBegin + RelativePosition;
			};
			auto GetRightChildPosition = [&](auto Position) {
				auto RelativePosition = Position - InitialBegin;
				RelativePosition <<= 1;
				RelativePosition += 2;
				return InitialBegin + RelativePosition;
			};
			auto GetParentPosition = [&](auto Position) {
				auto RelativePosition = Position - InitialBegin;
				RelativePosition -= 1;
				RelativePosition >>= 1;
				return InitialBegin + RelativePosition;
			};
			auto RestoreCorruptedHeap = [&](auto RootPosition, auto LastValidPosition) {
				auto LeftChildPosition = GetLeftChildPosition(RootPosition);
				auto RightChildPosition = GetRightChildPosition(RootPosition);
				auto SwappingPosition = RootPosition;
				auto RootHasAtLeastOneChild = [&]() {
					return LeftChildPosition <= LastValidPosition;
				};
				auto HeapHasNotBeenRestored = [&]() {
					auto RootHasRightChild = [&]() {
						return RightChildPosition <= LastValidPosition;
					};
					if (*SwappingPosition < *LeftChildPosition)
						SwappingPosition = LeftChildPosition;
					if (RootHasRightChild() && *SwappingPosition < *RightChildPosition)
						SwappingPosition = RightChildPosition;
					return SwappingPosition != RootPosition;
				};
				auto DownpropagateTheCorruption = [&]() {
					auto RecalibratePositions = [&]() {
						RootPosition = SwappingPosition;
						LeftChildPosition = GetLeftChildPosition(RootPosition);
						RightChildPosition = GetRightChildPosition(RootPosition);
					};
					Swap(*RootPosition, *SwappingPosition);
					RecalibratePositions();
				};
				while (RootHasAtLeastOneChild())
					if (HeapHasNotBeenRestored())
						DownpropagateTheCorruption();
					else
						return;
			};
			auto Heapify = [&]() {
				for (auto Cursor = GetParentPosition(InitialEnd - 1); Cursor >= InitialBegin; --Cursor)
					RestoreCorruptedHeap(Cursor, InitialEnd - 1);
			};
			Heapify();
			for (auto Cursor = InitialEnd - 1; Cursor > InitialBegin; --Cursor) {
				Swap(*Cursor, *InitialBegin);
				RestoreCorruptedHeap(InitialBegin, Cursor - 1);
			}
		};
		auto Quicksort = [&]() {
			auto Partition = [&]() {
				auto &Pivot = *Begin;
				auto ForwardCursor = Begin;
				auto BackwardCursor = End - 1;
				auto PartitionPositionFound = false;
				auto LocatePartitionPosition = [&]() {
					while (*ForwardCursor < Pivot)
						++ForwardCursor;
					while (Pivot < *BackwardCursor)
						--BackwardCursor;
					if (ForwardCursor >= BackwardCursor)
						PartitionPositionFound = true;
					else
						Swap(*ForwardCursor, *BackwardCursor);
				};
				auto MoveOnAndTryAgain = [&]() {
					++ForwardCursor;
					--BackwardCursor;
				};
				while (true) {
					LocatePartitionPosition();
					if (PartitionPositionFound)
						return BackwardCursor + 1;
					else
						MoveOnAndTryAgain();
				}
			};
			auto PartitionPosition = Partition();
			Sort(Begin, PartitionPosition, RecursionDepth + 1);
			Sort(PartitionPosition, End, RecursionDepth + 1);
		};
		if (Begin < End - 1)
			if (RecursionDepth == MaximumRecursionDepth)
				Heapsort();
			else
				Quicksort();
	}
}