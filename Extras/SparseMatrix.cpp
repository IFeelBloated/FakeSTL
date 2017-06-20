#include <iomanip>
#include "Helpers.hpp"
#include "List.hpp"
#include "Utility.hpp"
#include "MacroHacks.hpp"

namespace StandardTemplateLibrary::Extras {
	class SparseVector final {
		struct VectorNode final {
			self(Value, 0.);
			self(Index, 0_size);
			VectorNode() = default;
			VectorNode(double Value, std::size_t Index) {
				this->Value = Value;
				this->Index = Index;
			}
			VectorNode(VectorNode &&) = default;
			VectorNode(const VectorNode &) = default;
			auto operator=(VectorNode &&)->decltype(*this) = default;
			auto operator=(const VectorNode &)->decltype(*this) = default;
			~VectorNode() = default;
		};
		using Container = List<VectorNode>;
		self(ContainerPointer, static_cast<Container *>(nullptr));
		self(Dimension, 0_size);
	public:
		SparseVector() {
			ContainerPointer = new Container{};
		}
		SparseVector(std::initializer_list<decltype(0.)> Initialization) :SparseVector{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = ContainerPointer->Begin();
				while (Cursor != ContainerPointer->End())
					if (Cursor->Value == 0.)
						Cursor = ContainerPointer->Erase(Cursor);
					else
						++Cursor;
			};
			for (auto x : Initialization) {
				*ContainerPointer += {x, Cursor};
				++Cursor;
			}
			Dimension = ContainerPointer->Size();
			Compress();
		}
		auto &operator=(SparseVector &&OtherSparseVector) {
			if (this != &OtherSparseVector) {
				Swap(ContainerPointer, OtherSparseVector.ContainerPointer);
				Dimension = OtherSparseVector.Dimension;
			}
			return *this;
		}
		auto &operator=(const SparseVector &OtherSparseVector) {
			if (this != &OtherSparseVector) {
				*ContainerPointer = *OtherSparseVector.ContainerPointer;
				Dimension = OtherSparseVector.Dimension;
			}
			return *this;
		}
		SparseVector(SparseVector &&OtherSparseVector) {
			*this = Move(OtherSparseVector);
		}
		SparseVector(const SparseVector &OtherSparseVector) :SparseVector{} {
			*this = OtherSparseVector;
		}
		~SparseVector() {
			delete ContainerPointer;
		}
		auto Empty() const {
			return ContainerPointer->Empty();
		}
		auto Size() const {
			return Dimension;
		}
		auto Begin() {
			return ContainerPointer->Begin();
		}
		auto Begin() const {
			auto ConstantContainerPointer = const_cast<const Container *>(ContainerPointer);
			return ConstantContainerPointer->Begin();
		}
		auto End() {
			return ContainerPointer->End();
		}
		auto End() const {
			auto ConstantContainerPointer = const_cast<const Container *>(ContainerPointer);
			return ConstantContainerPointer->End();
		}
		static auto NullVector(std::size_t Dimension) {
			auto Vector = SparseVector{};
			Vector.Dimension = Dimension;
			return Vector;
		}
		auto &operator*=(double Value) {
			if (Value != 0.)
				for (auto &x : *this)
					x.Value *= Value;
			else
				ContainerPointer->Clear();
			return *this;
		}
		auto &operator+=(const SparseVector &RightHandSideVector) {
			auto Result = NullVector(Dimension);
			auto LeftHandSideCursor = Begin();
			auto RightHandSideCursor = RightHandSideVector.Begin();
			auto &ResultContainer = *Result.ContainerPointer;
			auto LeftHandSideVectorAlreadyTraversed = [&]() {
				return LeftHandSideCursor == End();
			};
			auto RightHandSideVectorAlreadyTraversed = [&]() {
				return RightHandSideCursor == RightHandSideVector.End();
			};
			auto ShiftTheLeftHandSideElementToResult = [&] {
				ResultContainer += *LeftHandSideCursor;
				++LeftHandSideCursor;
			};
			auto ShiftTheRightHandSideElementToResult = [&] {
				ResultContainer += *RightHandSideCursor;
				++RightHandSideCursor;
			};
			auto ElementWiseEvaluate = [&]() {
				auto AddUpAndAppendTheSumToResult = [&]() {
					auto Value = LeftHandSideCursor->Value + RightHandSideCursor->Value;
					if (Value != 0.)
						ResultContainer += { Value, LeftHandSideCursor->Index };
					++LeftHandSideCursor;
					++RightHandSideCursor;
				};
				while (LeftHandSideVectorAlreadyTraversed() == false && RightHandSideVectorAlreadyTraversed() == false)
					if (LeftHandSideCursor->Index == RightHandSideCursor->Index)
						AddUpAndAppendTheSumToResult();
					else if (LeftHandSideCursor->Index < RightHandSideCursor->Index)
						ShiftTheLeftHandSideElementToResult();
					else
						ShiftTheRightHandSideElementToResult();
			};
			auto ShiftWhatsLeftToResult = [&]() {
				while (LeftHandSideVectorAlreadyTraversed() == false)
					ShiftTheLeftHandSideElementToResult();
				while (RightHandSideVectorAlreadyTraversed() == false)
					ShiftTheRightHandSideElementToResult();
			};
			ElementWiseEvaluate();
			ShiftWhatsLeftToResult();
			*this = Move(Result);
			return *this;
		}
		friend auto operator*(double Value, SparseVector &&SomeVector) {
			return SomeVector *= Value;
		}
		friend auto operator*(double Value, const SparseVector &SomeVector) {
			return SparseVector{ SomeVector } *= Value;
		}
		friend auto operator*(SparseVector &&SomeVector, double Value) {
			return SomeVector *= Value;
		}
		friend auto operator*(const SparseVector &SomeVector, double Value) {
			return SparseVector{ SomeVector } *= Value;
		}
		friend auto &operator<<(std::ostream &Output, const SparseVector &SomeVector) {
			auto Cursor = SomeVector.Begin();
			auto PrintRealNumberInFormat = [&](auto Value) {
				Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << Value << " ";
			};
			for (auto Position = 0_size; Position < SomeVector.Dimension; ++Position)
				if (Cursor != SomeVector.ContainerPointer->End() && Cursor->Index == Position) {
					PrintRealNumberInFormat(Cursor->Value);
					++Cursor;
				}
				else
					PrintRealNumberInFormat(0.);
			return Output;
		}
	};

	class SparseMatrix final {
		struct MatrixNode final {
			self(Vector, SparseVector{});
			self(Index, 0_size);
			MatrixNode() = default;
			MatrixNode(SparseVector &&Vector, std::size_t Index) {
				this->Vector = Move(Vector);
				this->Index = Index;
			}
			MatrixNode(const SparseVector &Vector, std::size_t Index) {
				this->Vector = Vector;
				this->Index = Index;
			}
			auto &operator=(MatrixNode &&OtherMatrixNode) {
				if (this != &OtherMatrixNode) {
					Vector = Move(OtherMatrixNode.Vector);
					Index = OtherMatrixNode.Index;
				}
				return *this;
			}
			auto operator=(const MatrixNode &)->decltype(*this) = default;
			MatrixNode(MatrixNode &&OtherMatrixNode) {
				*this = Move(OtherMatrixNode);
			}
			MatrixNode(const MatrixNode &) = default;
			~MatrixNode() = default;
		};
		using Container = List<MatrixNode>;
		self(ContainerPointer, static_cast<Container *>(nullptr));
		self(Row, 0_size);
		self(Column, 0_size);
	public:
		SparseMatrix() {
			ContainerPointer = new Container{};
		}
		SparseMatrix(std::initializer_list<SparseVector> Initialization) :SparseMatrix{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = ContainerPointer->Begin();
				while (Cursor != ContainerPointer->End())
					if (Cursor->Vector.Empty())
						Cursor = ContainerPointer->Erase(Cursor);
					else
						++Cursor;
			};
			for (auto &x : Initialization) {
				*ContainerPointer += {x, Cursor};
				++Cursor;
			}
			Column = ContainerPointer->Begin()->Vector.Size();
			Row = ContainerPointer->Size();
			Compress();
		}
		auto &operator=(SparseMatrix &&OtherSparseMatrix) {
			if (this != &OtherSparseMatrix) {
				Swap(ContainerPointer, OtherSparseMatrix.ContainerPointer);
				Row = OtherSparseMatrix.Row;
				Column = OtherSparseMatrix.Column;
			}
			return *this;
		}
		auto &operator=(const SparseMatrix &OtherSparseMatrix) {
			if (this != &OtherSparseMatrix) {
				*ContainerPointer = *OtherSparseMatrix.ContainerPointer;
				Row = OtherSparseMatrix.Row;
				Column = OtherSparseMatrix.Column;
			}
			return *this;
		}
		SparseMatrix(SparseMatrix &&OtherSparseMatrix) {
			*this = Move(OtherSparseMatrix);
		}
		SparseMatrix(const SparseMatrix &OtherSparseMatrix) :SparseMatrix{} {
			*this = OtherSparseMatrix;
		}
		~SparseMatrix() {
			delete ContainerPointer;
		}
		static auto ZeroMatrix(std::size_t Row, std::size_t Column) {
			auto Matrix = SparseMatrix{};
			Matrix.Row = Row;
			Matrix.Column = Column;
			return Matrix;
		}
		auto &operator*=(const SparseMatrix &RightHandSideMatrix) {
			auto RightHandSideMatrixRowTable = reinterpret_cast<SparseVector **>(alloca(RightHandSideMatrix.Row * sizeof(void *)));
			auto Result = ZeroMatrix(Row, RightHandSideMatrix.Column);
			auto &ResultContainer = *Result.ContainerPointer;
			auto &LeftHandSideMatrixContainer = *ContainerPointer;
			auto InitializeRowTable = [&]() {
				auto Cursor = RightHandSideMatrix.ContainerPointer->Begin();
				auto RecordAndMoveOn = [&](auto Position) {
					auto GetNext = [](auto Iterator) {
						return ++Iterator;
					};
					RightHandSideMatrixRowTable[Position] = &Cursor->Vector;
					if (GetNext(Cursor) != RightHandSideMatrix.ContainerPointer->End())
						++Cursor;
				};
				for (auto Position = 0_size; Position < RightHandSideMatrix.Row; ++Position)
					if (Cursor->Index == Position)
						RecordAndMoveOn(Position);
					else
						RightHandSideMatrixRowTable[Position] = nullptr;
			};
			auto RowWiseMultiplication = [&](auto &Vector) {
				auto Result = SparseVector::NullVector(RightHandSideMatrix.Column);
				for (auto &x : Vector)
					if (RightHandSideMatrixRowTable[x.Index] != nullptr) {
						auto &RightHandSideRowVector = *RightHandSideMatrixRowTable[x.Index];
						Result += x.Value * RightHandSideRowVector;
					}
				return Result;
			};
			InitializeRowTable();
			for (auto &x : LeftHandSideMatrixContainer) {
				auto CurrentRow = RowWiseMultiplication(x.Vector);
				if (CurrentRow.Empty() == false)
					ResultContainer += { Move(CurrentRow), x.Index };
			}
			*this = Move(Result);
			return *this;
		}
		friend auto operator*(const SparseMatrix &MatrixA, const SparseMatrix &MatrixB) {
			return SparseMatrix{ MatrixA } *= MatrixB;
		}
		friend auto operator*(SparseMatrix &&MatrixA, const SparseMatrix &MatrixB) {
			return MatrixA *= MatrixB;
		}
		friend auto &operator<<(std::ostream &Output, const SparseMatrix &SomeMatrix) {
			auto Cursor = SomeMatrix.ContainerPointer->Begin();
			auto PrintNullVector = [&]() {
				for (auto Position = 0_size; Position < SomeMatrix.Column; ++Position)
					Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << 0. << " ";
				Output << std::endl;
			};
			for (auto Position = 0_size; Position < SomeMatrix.Row; ++Position)
				if (Cursor != SomeMatrix.ContainerPointer->End() && Cursor->Index == Position) {
					Output << Cursor->Vector << std::endl;
					++Cursor;
				}
				else
					PrintNullVector();
			return Output;
		}
	};
}

auto main()->int {
	using Matrix = StandardTemplateLibrary::Extras::SparseMatrix;
	auto A = Matrix{
		{ 1, 2, 0 },
		{ 3, 0, 1 },
		{ 0, 0, 0 } };
	auto B = Matrix{
		{ 0, 2, 0, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 3, 8, 0 } };
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "A = " << std::endl << A << std::endl;
	std::cout << "B = " << std::endl << B << std::endl;
	std::cout << "A * B = " << std::endl << A * B << std::endl;
	Pause();
}