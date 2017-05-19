#include <iostream>
#include "List.hpp"

struct PolynomialTerm final {
	decltype(0.) Coefficient = 0.;
	decltype(0.) Degree = 0.;
	PolynomialTerm(double Coefficient = 0., double Degree = 0.) {
		this->Coefficient = Coefficient;
		this->Degree = Degree;
	}
	PolynomialTerm(PolynomialTerm &&) = default;
	PolynomialTerm(const PolynomialTerm &) = default;
	auto operator=(PolynomialTerm &&)->PolynomialTerm & = default;
	auto operator=(const PolynomialTerm &)->PolynomialTerm & = default;
	~PolynomialTerm() = default;
	auto operator*(const PolynomialTerm &Operand) const {
		return PolynomialTerm{ Coefficient * Operand.Coefficient, Degree + Operand.Degree };
	}
	auto operator<(const PolynomialTerm &Operand) const {
		if (Degree != Operand.Degree)
			return Degree < Operand.Degree;
		else
			return Coefficient < Operand.Coefficient;
	}
};

auto main()->int {
	using Polynomial = StandardTemplateLibrary::List<PolynomialTerm>;
	auto PolynomialA = Polynomial{ { 1., 0. },{ 1., 1. } };
	auto PolynomialB = Polynomial{ { 1., 0. },{ 2., 1. },{ 6., 2. } };
	auto SimplifyPolynomial = [](auto &Object) {
		auto CombineLikeTerms = [&]() {
			auto Combine = [&]() {
				auto Cursor = Object.begin();
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				while (GetNext(Cursor) != Object.end())
					if (Cursor->Degree == GetNext(Cursor)->Degree) {
						Cursor->Coefficient += GetNext(Cursor)->Coefficient;
						Object.Erase(GetNext(Cursor));
					}
					else
						++Cursor;
			};
			Object.Sort();
			Combine();
		};
		auto Cleanup = [&]() {
			auto Cursor = Object.begin();
			while (Cursor != Object.end())
				if (Cursor->Coefficient == 0.)
					Cursor = Object.Erase(Cursor);
				else
					++Cursor;
		};
		CombineLikeTerms();
		Cleanup();
		Object.Reverse();
	};
	auto MultiplyPolynomial = [&]() {
		auto Result = Polynomial{};
		for (auto &x : PolynomialA)
			for (auto &y : PolynomialB)
				Result += x * y;
		SimplifyPolynomial(Result);
		return Result;
	};
	auto PrintPolynomial = [](auto &&Object) {
		auto &LastPolynomialTerm = *--Object.end();
		auto PrintCoefficient = [](auto &Value) {
			if (Value.Coefficient != 1.)
				std::cout << Value.Coefficient;
			else if (Value.Degree == 0.)
				std::cout << Value.Coefficient;
			else
				return;
		};
		auto PrintIndeterminate = [](auto &Value) {
			if (Value.Degree != 0. && Value.Degree != 1.)
				std::cout << "x^" << Value.Degree;
			else if (Value.Degree == 1.)
				std::cout << "x";
			else
				return;
		};
		if (Object.Empty())
			std::cout << "0";
		else
			for (auto &x : Object) {
				PrintCoefficient(x);
				PrintIndeterminate(x);
				if (&x != &LastPolynomialTerm)
					std::cout << " + ";
			}
		std::cout << std::endl;
	};
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "PolynomialA: ";
	SimplifyPolynomial(PolynomialA);
	PrintPolynomial(PolynomialA);
	std::cout << "PolynomialB: ";
	SimplifyPolynomial(PolynomialB);
	PrintPolynomial(PolynomialB);
	std::cout << "PolynomialA * PolynomialB: ";
	PrintPolynomial(MultiplyPolynomial());
	Pause();
}
