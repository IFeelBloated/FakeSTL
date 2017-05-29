#include <iomanip>
#include "Helpers.hpp"
#include "List.hpp"

namespace StandardTemplateLibrary::Extras {
	class SparseVector final {
		struct VectorNode final {
			decltype(0.) Value = 0.;
			decltype(0_size) Index = 0;
			VectorNode() = default;
			VectorNode(double Value, std::size_t Index) {
				this->Value = Value;
				this->Index = Index;
			}
			VectorNode(VectorNode &&) = default;
			VectorNode(const VectorNode &) = default;
			auto operator=(VectorNode &&)->VectorNode & = default;
			auto operator=(const VectorNode &)->VectorNode & = default;
			~VectorNode() = default;
		};
		using Container = List<VectorNode>;
		Container *ContainerPointer = nullptr;
		decltype(0_size) Dimension = 0;
	public:
		SparseVector() {
			ContainerPointer = new Container{};
		}
		SparseVector(std::initializer_list<decltype(0.)> Initialization) :SparseVector{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = ContainerPointer->begin();
				while (Cursor != ContainerPointer->end())
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
				auto TemporaryPointer = ContainerPointer;
				ContainerPointer = OtherSparseVector.ContainerPointer;
				OtherSparseVector.ContainerPointer = TemporaryPointer;
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
			*this = static_cast<SparseVector &&>(OtherSparseVector);
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
		auto begin() {
			return ContainerPointer->begin();
		}
		auto begin() const {
			auto ConstantContainerPointer = const_cast<const Container *>(ContainerPointer);
			return ConstantContainerPointer->begin();
		}
		auto end() {
			return ContainerPointer->end();
		}
		auto end() const {
			auto ConstantContainerPointer = const_cast<const Container *>(ContainerPointer);
			return ConstantContainerPointer->end();
		}
		static auto NullVector(std::size_t Dimension) {
			auto Vector = SparseVector{};
			Vector.Dimension = Dimension;
			return Vector;
		}
		auto &operator*=(double Value) {
			for (auto &x : *ContainerPointer)
				x.Value *= Value;
			return *this;
		}
		auto &operator+=(const SparseVector &RightHandSideVector) {
			auto Result = NullVector(Dimension);
			auto LeftHandSideCursor = ContainerPointer->begin();
			auto RightHandSideCursor = RightHandSideVector.ContainerPointer->begin();
			auto LeftHandSideVectorActivated = [&](auto Cursor) {
				return LeftHandSideCursor != ContainerPointer->end() && LeftHandSideCursor->Index == Cursor;
			};
			auto RightHandSideVectorActivated = [&](auto Cursor) {
				return RightHandSideCursor != RightHandSideVector.ContainerPointer->end() && RightHandSideCursor->Index == Cursor;
			};
			auto AddUpAndShiftTheSumToResult = [&]() {
				auto Value = LeftHandSideCursor->Value + RightHandSideCursor->Value;
				if (Value != 0.)
					*Result.ContainerPointer += { Value, LeftHandSideCursor->Index };
				++LeftHandSideCursor;
				++RightHandSideCursor;
			};
			auto ShiftTheLeftHandSideElementToResult = [&] {
				*Result.ContainerPointer += *LeftHandSideCursor;
				++LeftHandSideCursor;
			};
			auto ShiftTheRightHandSideElementToResult = [&] {
				*Result.ContainerPointer += *RightHandSideCursor;
				++RightHandSideCursor;
			};
			for (auto Cursor = 0_size; Cursor < Dimension; ++Cursor)
				if (LeftHandSideVectorActivated(Cursor) && RightHandSideVectorActivated(Cursor))
					AddUpAndShiftTheSumToResult();
				else if (LeftHandSideVectorActivated(Cursor))
					ShiftTheLeftHandSideElementToResult();
				else if (RightHandSideVectorActivated(Cursor))
					ShiftTheRightHandSideElementToResult();
			*this = static_cast<SparseVector &&>(Result);
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
			auto Cursor = SomeVector.ContainerPointer->begin();
			auto PrintRealNumberInFormat = [&](auto Value) {
				Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << Value << " ";
			};
			for (auto Position = 0_size; Position < SomeVector.Dimension; ++Position)
				if (Cursor != SomeVector.ContainerPointer->end() && Cursor->Index == Position) {
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
			SparseVector Vector = {};
			decltype(0_size) Index = 0;
			MatrixNode() = default;
			MatrixNode(SparseVector &&Vector, std::size_t Index) {
				this->Vector = static_cast<SparseVector &&>(Vector);
				this->Index = Index;
			}
			MatrixNode(const SparseVector &Vector, std::size_t Index) {
				this->Vector = Vector;
				this->Index = Index;
			}
			MatrixNode(MatrixNode &&OtherMatrixNode) :MatrixNode{ static_cast<SparseVector &&>(OtherMatrixNode.Vector), OtherMatrixNode.Index } {}
			MatrixNode(const MatrixNode &) = default;
			auto &operator=(MatrixNode &&OtherMatrixNode) {
				if (this != &OtherMatrixNode)
					new(this) MatrixNode{ static_cast<SparseVector &&>(OtherMatrixNode.Vector), OtherMatrixNode.Index };
				return *this;
			}
			auto operator=(const MatrixNode &)->MatrixNode & = default;
			~MatrixNode() = default;
		};
		using Container = List<MatrixNode>;
		Container *ContainerPointer = nullptr;
		decltype(0_size) Row = 0;
		decltype(0_size) Column = 0;
	public:
		SparseMatrix() {
			ContainerPointer = new Container{};
		}
		SparseMatrix(std::initializer_list<SparseVector> Initialization) :SparseMatrix{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = ContainerPointer->begin();
				while (Cursor != ContainerPointer->end())
					if (Cursor->Vector.Empty())
						Cursor = ContainerPointer->Erase(Cursor);
					else
						++Cursor;
			};
			for (auto &x : Initialization) {
				*ContainerPointer += {x, Cursor};
				++Cursor;
			}
			Column = ContainerPointer->begin()->Vector.Size();
			Row = ContainerPointer->Size();
			Compress();
		}
		auto &operator=(SparseMatrix &&OtherSparseMatrix) {
			if (this != &OtherSparseMatrix) {
				auto TemporaryPointer = ContainerPointer;
				ContainerPointer = OtherSparseMatrix.ContainerPointer;
				OtherSparseMatrix.ContainerPointer = TemporaryPointer;
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
			*this = static_cast<SparseMatrix &&>(OtherSparseMatrix);
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
			auto InitializeRowTable = [&]() {
				auto Cursor = RightHandSideMatrix.ContainerPointer->begin();
				auto RecordAndMoveOn = [&](auto Position) {
					auto GetNext = [](auto Iterator) {
						return ++Iterator;
					};
					RightHandSideMatrixRowTable[Position] = &Cursor->Vector;
					if (GetNext(Cursor) != RightHandSideMatrix.ContainerPointer->end())
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
			for (auto &x : *ContainerPointer) {
				auto CurrentRow = RowWiseMultiplication(x.Vector);
				if (!CurrentRow.Empty())
					*Result.ContainerPointer += { static_cast<SparseVector &&>(CurrentRow), x.Index };
			}
			*this = static_cast<SparseMatrix &&>(Result);
			return *this;
		}
		friend auto operator*(const SparseMatrix &MatrixA, const SparseMatrix &MatrixB) {
			return SparseMatrix{ MatrixA } *= MatrixB;
		}
		friend auto operator*(SparseMatrix &&MatrixA, const SparseMatrix &MatrixB) {
			return MatrixA *= MatrixB;
		}
		friend auto &operator<<(std::ostream &Output, const SparseMatrix &SomeMatrix) {
			auto Cursor = SomeMatrix.ContainerPointer->begin();
			auto PrintNullVector = [&]() {
				for (auto Position = 0_size; Position < SomeMatrix.Column; ++Position)
					Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << 0. << " ";
				Output << std::endl;
			};
			for (auto Position = 0_size; Position < SomeMatrix.Row; ++Position)
				if (Cursor != SomeMatrix.ContainerPointer->end() && Cursor->Index == Position) {
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