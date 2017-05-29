#pragma once
#include "Helpers.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary {
	template<typename RandomAccessIterator>
	auto Sort(RandomAccessIterator Begin, RandomAccessIterator End)->void {
		auto Partition = [&]() {
			auto &Pivot = *Begin;
			auto ForwardCursor = Begin;
			auto BackwardCursor = End - 1;
			auto PartitionPositionFound = false;
			auto LocatePartitionPosition = [&]() {
				while (*ForwardCursor < Pivot)
					++ForwardCursor;
				while (*BackwardCursor > Pivot)
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
		if (Begin < End - 1) {
			auto PartitionPosition = Partition();
			Sort(Begin, PartitionPosition);
			Sort(PartitionPosition, End);
		}
	}
}