#include <iostream>
#include "List.hpp"
#include "MacroHacks.hpp"

struct PolynomialTerm final {
	self(Coefficient, 0.);
	self(Degree, 0.);
	PolynomialTerm(double Coefficient = 0., double Degree = 0.) {
		this->Coefficient = Coefficient;
		this->Degree = Degree;
	}
	PolynomialTerm(PolynomialTerm &&) = default;
	PolynomialTerm(const PolynomialTerm &) = default;
	auto operator=(PolynomialTerm &&)->decltype(*this) = default;
	auto operator=(const PolynomialTerm &)->decltype(*this) = default;
	~PolynomialTerm() = default;
	auto operator*(const PolynomialTerm &RightHandSideTerm) const {
		return PolynomialTerm{ Coefficient * RightHandSideTerm.Coefficient, Degree + RightHandSideTerm.Degree };
	}
	auto operator<(const PolynomialTerm &RightHandSideTerm) const {
		if (Degree != RightHandSideTerm.Degree)
			return Degree < RightHandSideTerm.Degree;
		else
			return Coefficient < RightHandSideTerm.Coefficient;
	}
};

auto main()->int {
	using Polynomial = StandardTemplateLibrary::List<PolynomialTerm>;
	auto PolynomialA = Polynomial{ { 1., 0. }, { 1., 1. } };
	auto PolynomialB = Polynomial{ { 1., 0. }, { 2., 1. }, { 6., 2. } };
	auto SimplifyPolynomial = [](auto &Expression) {
		auto CombineLikeTerms = [&]() {
			auto Combine = [&]() {
				auto Cursor = Expression.Begin();
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				while (GetNext(Cursor) != Expression.End())
					if (Cursor->Degree == GetNext(Cursor)->Degree) {
						Cursor->Coefficient += GetNext(Cursor)->Coefficient;
						Expression.Erase(GetNext(Cursor));
					}
					else
						++Cursor;
			};
			Expression.Sort();
			Combine();
		};
		auto Cleanup = [&]() {
			auto Cursor = Expression.Begin();
			while (Cursor != Expression.End())
				if (Cursor->Coefficient == 0.)
					Cursor = Expression.Erase(Cursor);
				else
					++Cursor;
		};
		CombineLikeTerms();
		Cleanup();
		Expression.Reverse();
	};
	auto MultiplyPolynomial = [&]() {
		auto Result = Polynomial{};
		for (auto &x : PolynomialA)
			for (auto &y : PolynomialB)
				Result += x * y;
		SimplifyPolynomial(Result);
		return Result;
	};
	auto PrintPolynomial = [](auto &&Expression) {
		auto &LastPolynomialTerm = *--Expression.End();
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
		if (Expression.Empty())
			std::cout << "0";
		else
			for (auto &x : Expression) {
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
