#include <iomanip>
#include "Helpers.hpp"
#include "List.hpp"

namespace StandardTemplateLibrary::Extras {
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

	class SparseVector final {
		using Container = List<VectorNode>;
		Container *Pointer = nullptr;
		decltype(0_size) Dimension = 0;
	public:
		SparseVector() {
			Pointer = new Container{};
		}
		SparseVector(std::initializer_list<decltype(0.)> Initialization) :SparseVector{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = Pointer->begin();
				while (Cursor != Pointer->end())
					if (Cursor->Value == 0.)
						Cursor = Pointer->Erase(Cursor);
					else
						++Cursor;
			};
			for (auto x : Initialization) {
				*Pointer += {x, Cursor};
				++Cursor;
			}
			Dimension = Pointer->Size();
			Compress();
		}
		auto &operator=(SparseVector &&Object) {
			if (this != &Object) {
				auto TemporaryPointer = Pointer;
				Pointer = Object.Pointer;
				Object.Pointer = TemporaryPointer;
				Dimension = Object.Dimension;
			}
			return *this;
		}
		auto &operator=(const SparseVector &Object) {
			if (this != &Object) {
				*Pointer = *Object.Pointer;
				Dimension = Object.Dimension;
			}
			return *this;
		}
		SparseVector(SparseVector &&Object) {
			*this = static_cast<SparseVector &&>(Object);
		}
		SparseVector(const SparseVector &Object) :SparseVector{} {
			*this = Object;
		}
		~SparseVector() {
			delete Pointer;
		}
		auto Empty() const {
			return Pointer->Empty();
		}
		auto Size() const {
			return Dimension;
		}
		auto &Data() {
			return *Pointer;
		}
		const auto &Data() const {
			return *Pointer;
		}
		static auto NullVector(std::size_t Dimension) {
			auto Vector = SparseVector{};
			Vector.Dimension = Dimension;
			return Vector;
		}
		auto &operator*=(double Value) {
			for (auto &x : *Pointer)
				x.Value *= Value;
			return *this;
		}
		auto &operator+=(const SparseVector &Object) {
			auto Result = NullVector(Dimension);
			auto LeftHandSideCursor = Pointer->begin();
			auto RightHandSideCursor = Object.Pointer->begin();
			auto LeftHandSideVectorActivated = [&](auto Cursor) {
				return LeftHandSideCursor != Pointer->end() && LeftHandSideCursor->Index == Cursor;
			};
			auto RightHandSideVectorActivated = [&](auto Cursor) {
				return RightHandSideCursor != Object.Pointer->end() && RightHandSideCursor->Index == Cursor;
			};
			auto AddUpAndShiftTheSumToResult = [&]() {
				auto Value = LeftHandSideCursor->Value + RightHandSideCursor->Value;
				if (Value != 0.)
					*Result.Pointer += { Value, LeftHandSideCursor->Index };
				++LeftHandSideCursor;
				++RightHandSideCursor;
			};
			auto ShiftTheLeftHandSideElementToResult = [&] {
				*Result.Pointer += *LeftHandSideCursor;
				++LeftHandSideCursor;
			};
			auto ShiftTheRightHandSideElementToResult = [&] {
				*Result.Pointer += *RightHandSideCursor;
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
		friend auto operator*(double Value, SparseVector &&Object) {
			return Object *= Value;
		}
		friend auto operator*(double Value, const SparseVector &Object) {
			return SparseVector{ Object } *= Value;
		}
		friend auto operator*(SparseVector &&Object, double Value) {
			return Object *= Value;
		}
		friend auto operator*(const SparseVector &Object, double Value) {
			return SparseVector{ Object } *= Value;
		}
		friend auto &operator<<(std::ostream &Output, const SparseVector &Object) {
			auto Cursor = Object.Pointer->begin();
			auto PrintRealNumberInFormat = [&](auto Value) {
				Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << Value << " ";
			};
			for (auto Position = 0_size; Position < Object.Dimension; ++Position)
				if (Cursor != Object.Pointer->end() && Cursor->Index == Position) {
					PrintRealNumberInFormat(Cursor->Value);
					++Cursor;
				}
				else
					PrintRealNumberInFormat(0.);
			return Output;
		}
	};

	struct MatrixNode final {
		SparseVector Value = {};
		decltype(0_size) Index = 0;
		MatrixNode() = default;
		MatrixNode(SparseVector &&Value, std::size_t Index) {
			this->Value = static_cast<SparseVector &&>(Value);
			this->Index = Index;
		}
		MatrixNode(const SparseVector &Value, std::size_t Index) {
			this->Value = Value;
			this->Index = Index;
		}
		MatrixNode(MatrixNode &&Object) :MatrixNode{ static_cast<SparseVector &&>(Object.Value), Object.Index } {}
		MatrixNode(const MatrixNode &Object) = default;
		auto &operator=(MatrixNode &&Object) {
			if (this != &Object)
				new(this) MatrixNode{ static_cast<SparseVector &&>(Object.Value), Object.Index };
			return *this;
		}
		auto operator=(const MatrixNode &)->MatrixNode & = default;
		~MatrixNode() = default;
	};

	class SparseMatrix final {
		using Container = List<MatrixNode>;
		Container *Pointer = nullptr;
		decltype(0_size) Row = 0;
		decltype(0_size) Column = 0;
	public:
		SparseMatrix() {
			Pointer = new Container{};
		}
		SparseMatrix(std::initializer_list<SparseVector> Initialization) :SparseMatrix{} {
			auto Cursor = 0_size;
			auto Compress = [this]() {
				auto Cursor = Pointer->begin();
				while (Cursor != Pointer->end())
					if (Cursor->Value.Empty())
						Cursor = Pointer->Erase(Cursor);
					else
						++Cursor;
			};
			for (auto &x : Initialization) {
				*Pointer += {x, Cursor};
				++Cursor;
			}
			Column = Pointer->begin()->Value.Size();
			Row = Pointer->Size();
			Compress();
		}
		auto &operator=(SparseMatrix &&Object) {
			if (this != &Object) {
				auto TemporaryPointer = Pointer;
				Pointer = Object.Pointer;
				Object.Pointer = TemporaryPointer;
				Row = Object.Row;
				Column = Object.Column;
			}
			return *this;
		}
		auto &operator=(const SparseMatrix &Object) {
			if (this != &Object) {
				*Pointer = *Object.Pointer;
				Row = Object.Row;
				Column = Object.Column;
			}
			return *this;
		}
		SparseMatrix(SparseMatrix &&Object) {
			*this = static_cast<SparseMatrix &&>(Object);
		}
		SparseMatrix(const SparseMatrix &Object) :SparseMatrix{} {
			*this = Object;
		}
		~SparseMatrix() {
			delete Pointer;
		}
		static auto ZeroMatrix(std::size_t Row, std::size_t Column) {
			auto Matrix = SparseMatrix{};
			Matrix.Row = Row;
			Matrix.Column = Column;
			return Matrix;
		}
		auto &operator*=(const SparseMatrix &Object) {
			auto RightHandSideOperandRowTable = reinterpret_cast<SparseVector **>(alloca(Object.Row * sizeof(void *)));
			auto Result = ZeroMatrix(Row, Object.Column);
			auto InitializeRowTable = [&]() {
				auto Cursor = Object.Pointer->begin();
				auto RecordAndMoveOn = [&](auto Position) {
					auto GetNext = [](auto Iterator) {
						return ++Iterator;
					};
					RightHandSideOperandRowTable[Position] = &Cursor->Value;
					if (GetNext(Cursor) != Object.Pointer->end())
						++Cursor;
				};
				for (auto Position = 0_size; Position < Object.Row; ++Position)
					if (Cursor->Index == Position)
						RecordAndMoveOn(Position);
					else
						RightHandSideOperandRowTable[Position] = nullptr;
			};
			auto RowWiseMultiplication = [&](auto &Vector) {
				auto Result = SparseVector::NullVector(Object.Column);
				for (auto &x : Vector.Data())
					if (RightHandSideOperandRowTable[x.Index] != nullptr) {
						auto &RightHandSideRowVector = *RightHandSideOperandRowTable[x.Index];
						Result += x.Value * RightHandSideRowVector;
					}
				return Result;
			};
			InitializeRowTable();
			for (auto &x : *Pointer) {
				auto CurrentRow = RowWiseMultiplication(x.Value);
				if (!CurrentRow.Empty())
					*Result.Pointer += { static_cast<SparseVector &&>(CurrentRow), x.Index };
			}
			*this = static_cast<SparseMatrix &&>(Result);
			return *this;
		}
		friend auto operator*(const SparseMatrix &ObjectA, const SparseMatrix &ObjectB) {
			return SparseMatrix{ ObjectA } *= ObjectB;
		}
		friend auto operator*(SparseMatrix &&ObjectA, const SparseMatrix &ObjectB) {
			return ObjectA *= ObjectB;
		}
		friend auto &operator<<(std::ostream &Output, const SparseMatrix &Object) {
			auto Cursor = Object.Pointer->begin();
			auto PrintNullVector = [&]() {
				for (auto i = 0_size; i < Object.Column; ++i)
					Output << std::setiosflags(std::ios::fixed) << std::setprecision(3) << 0. << " ";
				Output << std::endl;
			};
			for (auto Position = 0_size; Position < Object.Row; ++Position)
				if (Cursor != Object.Pointer->end() && Cursor->Index == Position) {
					Output << Cursor->Value << std::endl;
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