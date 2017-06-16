#include <iostream>
#include "Helpers.hpp"
#include "Utility.hpp"

namespace StandardTemplateLibrary::Extras {
	class Graph final {
		decltype(0.) *AdjacencyMatrix = nullptr;
		decltype(0_size) VertexCount = 0;
		decltype(false) IsDirectedGraph = false;
		static constexpr auto Infinity = 1e+308 * 1e+308;
	public:
		Graph(std::size_t VertexCount, bool IsDirectedGraph = false) {
			auto AdjacencyMatrixSize = VertexCount * VertexCount;
			AdjacencyMatrix = new double[AdjacencyMatrixSize];
			this->VertexCount = VertexCount;
			this->IsDirectedGraph = IsDirectedGraph;
			for (auto Cursor = 0_size; Cursor < AdjacencyMatrixSize; ++Cursor)
				AdjacencyMatrix[Cursor] = Infinity;
		}
		auto &operator=(Graph &&OtherGraph) {
			if (this != &OtherGraph) {
				Swap(AdjacencyMatrix, OtherGraph.AdjacencyMatrix);
				VertexCount = OtherGraph.VertexCount;
				IsDirectedGraph = OtherGraph.IsDirectedGraph;
			}
			return *this;
		}
		auto &operator=(const Graph &OtherGraph) {
			if (this != &OtherGraph) {
				delete[] AdjacencyMatrix;
				VertexCount = OtherGraph.VertexCount;
				IsDirectedGraph = OtherGraph.IsDirectedGraph;
				auto AdjacencyMatrixSize = VertexCount * VertexCount;
				AdjacencyMatrix = new double[AdjacencyMatrixSize];
				std::memcpy(AdjacencyMatrix, OtherGraph.AdjacencyMatrix, AdjacencyMatrixSize * sizeof(double));
			}
			return *this;
		}
		Graph(Graph &&OtherGraph) {
			*this = Move(OtherGraph);
		}
		Graph(const Graph &OtherGraph) {
			*this = OtherGraph;
		}
		~Graph() {
			delete[] AdjacencyMatrix;
		}
		auto operator[](std::ptrdiff_t Row) {
			return AdjacencyMatrix + Row * VertexCount;
		}
		auto operator[](std::ptrdiff_t Row) const {
			return CastToConstantPointer(AdjacencyMatrix + Row * VertexCount);
		}
		auto Connect(std::size_t StartingOffVertexPosition, std::size_t DestinationVertexPosition, double Distance) {
			auto &Self = *this;
			Self[StartingOffVertexPosition][DestinationVertexPosition] = Distance;
			if (IsDirectedGraph == false)
				Self[DestinationVertexPosition][StartingOffVertexPosition] = Distance;
		}
		auto GenerateMinimumSpanningTree(std::size_t StartingOffVertexPosition) const {
			struct ShortestAvailablePath final {
				decltype(0_size) DestinationVertexPosition = 0;
				decltype(0.) Distance = 0.;
				ShortestAvailablePath() = default;
				ShortestAvailablePath(std::size_t DestinationVertexPosition, double Distance) {
					this->DestinationVertexPosition = DestinationVertexPosition;
					this->Distance = Distance;
				}
				ShortestAvailablePath(ShortestAvailablePath &&) = default;
				ShortestAvailablePath(const ShortestAvailablePath &) = default;
				auto operator=(ShortestAvailablePath &&)->decltype(*this) = default;
				auto operator=(const ShortestAvailablePath &)->decltype(*this) = default;
				~ShortestAvailablePath() = default;
			};
			auto &Self = *this;
			auto ShortestPathsToTheExistingVertices = reinterpret_cast<ShortestAvailablePath *>(alloca(VertexCount * sizeof(ShortestAvailablePath)));
			constexpr auto VertexAlreadyCaptured = -Infinity;
			auto MinimumSpanningTreeConstructionCompleted = false;
			auto Result = Graph{ VertexCount, false };
			auto InitalizeShortestPathsArray = [&]() {
				for (auto Cursor = 0_size; Cursor < VertexCount; ++Cursor)
					ShortestPathsToTheExistingVertices[Cursor] = { StartingOffVertexPosition, Self[StartingOffVertexPosition][Cursor] };
				ShortestPathsToTheExistingVertices[StartingOffVertexPosition].Distance = VertexAlreadyCaptured;
			};
			auto AppendANewVertexToTheMinimumSpanningTree = [&]() {
				auto LocateTheNextTargetVertex = [&]() {
					auto VertexPosition = StartingOffVertexPosition;
					auto TemporarilyResetTheDistanceAtStartingOffVertexPosition = [&]() {
						ShortestPathsToTheExistingVertices[StartingOffVertexPosition].Distance = Infinity;
					};
					auto RestoreTheDistanceAtStartingOffVertexPosition = [&]() {
						ShortestPathsToTheExistingVertices[StartingOffVertexPosition].Distance = VertexAlreadyCaptured;
					};
					auto CheckValidityForTheCandidatePosition = [&](auto CandidatePosition) {
						auto CurrentDistance = ShortestPathsToTheExistingVertices[CandidatePosition].Distance;
						auto ReferenceDistance = ShortestPathsToTheExistingVertices[VertexPosition].Distance;
						return CurrentDistance != VertexAlreadyCaptured && CurrentDistance < ReferenceDistance;
					};
					TemporarilyResetTheDistanceAtStartingOffVertexPosition();
					for (auto Cursor = 0_size; Cursor < VertexCount; ++Cursor)
						if (CheckValidityForTheCandidatePosition(Cursor) == true)
							VertexPosition = Cursor;
					RestoreTheDistanceAtStartingOffVertexPosition();
					return VertexPosition;
				};
				auto NextVertexPosition = LocateTheNextTargetVertex();
				auto CheckValidityForTheNextVertexPosition = [&]() {
					return NextVertexPosition != StartingOffVertexPosition;
				};
				auto UpdateShortestPathsArray = [&]() {
					for (auto Cursor = 0_size; Cursor < VertexCount; ++Cursor)
						if (Self[NextVertexPosition][Cursor] < ShortestPathsToTheExistingVertices[Cursor].Distance)
							ShortestPathsToTheExistingVertices[Cursor] = { NextVertexPosition, Self[NextVertexPosition][Cursor] };
				};
				if (CheckValidityForTheNextVertexPosition() == true) {
					auto CurrentVertexPosition = ShortestPathsToTheExistingVertices[NextVertexPosition].DestinationVertexPosition;
					auto &CorrespondingDistance = ShortestPathsToTheExistingVertices[NextVertexPosition].Distance;
					Result.Connect(NextVertexPosition, CurrentVertexPosition, CorrespondingDistance);
					CorrespondingDistance = VertexAlreadyCaptured;
					UpdateShortestPathsArray();
				}
				else
					MinimumSpanningTreeConstructionCompleted = true;
			};
			if (IsDirectedGraph == false) {
				InitalizeShortestPathsArray();
				while (MinimumSpanningTreeConstructionCompleted == false)
					AppendANewVertexToTheMinimumSpanningTree();
			}
			return Result;
		}
		friend auto &operator<<(std::ostream &Output, const Graph &SomeGraph) {
			auto PrintConditionally = [&](auto RowCursor, auto ColumnCursor) {
				if (SomeGraph[RowCursor][ColumnCursor] != Graph::Infinity)
					Output << "vertex " << RowCursor << " is connected to vertex " << ColumnCursor << ", distance = " << SomeGraph[RowCursor][ColumnCursor] << std::endl;
			};
			if (SomeGraph.IsDirectedGraph)
				for (auto RowCursor = 0_size; RowCursor < SomeGraph.VertexCount; ++RowCursor)
					for (auto ColumnCursor = 0_size; ColumnCursor < SomeGraph.VertexCount; ++ColumnCursor)
						PrintConditionally(RowCursor, ColumnCursor);
			else
				for (auto RowCursor = 0_size; RowCursor < SomeGraph.VertexCount; ++RowCursor)
					for (auto ColumnCursor = RowCursor; ColumnCursor < SomeGraph.VertexCount; ++ColumnCursor)
						PrintConditionally(RowCursor, ColumnCursor);
			return Output;
		}
	};
}

auto main()->int {
	using Graph = StandardTemplateLibrary::Extras::Graph;
	auto TestGraph = Graph{ 8 };
	auto GenerateTestGraph = [&]() {
		TestGraph.Connect(1, 2, 28.);
		TestGraph.Connect(1, 3, 10.);
		TestGraph.Connect(2, 4, 14.);
		TestGraph.Connect(2, 5, 16.);
		TestGraph.Connect(3, 6, 25.);
		TestGraph.Connect(4, 6, 24.);
		TestGraph.Connect(4, 7, 18.);
		TestGraph.Connect(5, 7, 12.);
		TestGraph.Connect(6, 7, 22.);
	};
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	GenerateTestGraph();
	std::cout << "Test Graph:" << std::endl << TestGraph << std::endl;
	std::cout << "Corresponding Minimum Spanning Tree:" << std::endl << TestGraph.GenerateMinimumSpanningTree(1) << std::endl;
	Pause();
}