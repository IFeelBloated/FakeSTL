#include <iostream>
#include <cstdlib>
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
	auto PolynomialA = Polynomial{ PolynomialTerm{ 1., 0. }, PolynomialTerm{ 1., 1. } };
	auto PolynomialB = Polynomial{ PolynomialTerm{ 1., 0. }, PolynomialTerm{ 2., 1. }, PolynomialTerm{ 6., 2. } };
	auto SimplifyPolynomial = [](auto &Object) {
		auto CombineLikeTerms = [&]() {
			auto Combine = [&]() {
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				for (auto i = Object.begin(); i != Object.end(); ++i)
					for (auto j = GetNext(i); j != Object.end() && i->Degree == j->Degree;) {
						i->Coefficient += j->Coefficient;
						j = Object.Erase(j);
					}
			};
			Object.Sort();
			Combine();
		};
		auto Cleanup = [&]() {
			for (auto i = Object.begin(); i != Object.end();) {
				if (i->Coefficient == 0.)
					i = Object.Erase(i);
				else
					++i;
			}
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
	std::cout << "PolynomialA: ";
	SimplifyPolynomial(PolynomialA);
	PrintPolynomial(PolynomialA);
	std::cout << "PolynomialB: ";
	SimplifyPolynomial(PolynomialB);
	PrintPolynomial(PolynomialB);
	std::cout << "PolynomialA * PolynomialB: ";
	PrintPolynomial(MultiplyPolynomial());
	system("pause");
}
