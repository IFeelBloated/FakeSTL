#include <iostream>
#include <cstdlib>
#include "Stack.hpp"
#include "List.hpp"

union DataSet final {
	decltype(0.) Operand = 0.;
	decltype('0') Operator;
	DataSet() = default;
	DataSet(double Value) {
		Operand = Value;
	}
	DataSet(char Value) {
		Operator = Value;
	}
	DataSet(const DataSet &) = default;
	DataSet(DataSet &&) = default;
	auto operator=(const DataSet &)->DataSet & = default;
	auto operator=(DataSet &&)->DataSet & = default;
	~DataSet() = default;
};

struct Node final {
	DataSet Value = {};
	decltype(0) Precedence = 0;
	decltype(false) IsOperator = false;
	Node() = default;
	Node(double Value) {
		this->Value = Value;
	}
	Node(char Value) {
		auto GetPrecedence = [&]() {
			switch (Value) {
			case '+': case '-':
				return 1;
			case '*': case '/':
				return 2;
			default:
				return 0;
			}
		};
		this->Value = Value;
		IsOperator = true;
		Precedence = GetPrecedence();
	}
	Node(const Node &) = default;
	Node(Node &&) = default;
	auto operator=(const Node &)->Node & = default;
	auto operator=(Node &&)->Node & = default;
	~Node() = default;
	auto operator==(char Value) const {
		return this->Value.Operator == Value;
	}
	auto operator!=(char Value) const {
		return this->Value.Operator != Value;
	}
	friend auto &operator<<(std::ostream &Output, const Node &Object) {
		if (Object.IsOperator)
			Output << Object.Value.Operator;
		else
			Output << Object.Value.Operand;
		return Output;
	}
};

auto main()->int {
	using Expression = StandardTemplateLibrary::List<Node>;
	auto InfixExpression = Expression{ '(', 12., '-', 8., ')', '/', 4., '+', 2., '*', 3. };
	auto RPNExpression = Expression{};
	auto GetRPNExpression = [&]() {
		using ExpressionStack = StandardTemplateLibrary::Stack<Node, Expression>;
		auto OperatorStack = ExpressionStack{ 0.0 };
		auto PopToRPNExpression = [&]() {
			RPNExpression += OperatorStack.Top();
			--OperatorStack;
		};
		auto OperandToRPNExpression = [&](auto &Object) {
			RPNExpression += Object;
		};
		auto OperatorToRPNExpression = [&](auto &Object) {
			if (Object == '(')
				OperatorStack += Object;
			else if (Object == ')') {
				while (OperatorStack.Top() != '(')
					PopToRPNExpression();
				--OperatorStack;
			}
			else
				if (Object.Precedence > OperatorStack.Top().Precedence)
					OperatorStack += Object;
				else {
					while (OperatorStack.Size() > 1 && Object.Precedence <= OperatorStack.Top().Precedence)
						PopToRPNExpression();
					OperatorStack += Object;
				}
		};
		auto StackCleanup = [&]() {
			while (OperatorStack.Size() > 1)
				PopToRPNExpression();
		};
		for (auto &x : InfixExpression)
			if (x.IsOperator)
				OperatorToRPNExpression(x);
			else
				OperandToRPNExpression(x);
		StackCleanup();
	};
	auto EvaluateRPNExpression = [&]() {
		using ExpressionStack = StandardTemplateLibrary::Stack<decltype(0.), StandardTemplateLibrary::List<decltype(0.)>>;
		auto OperandStack = ExpressionStack{};
		auto Evaluate = [&](auto &Object) {
			auto Operation = [&](auto &&Operator) {
				auto RightHandOperand = OperandStack.Top();
				--OperandStack;
				Operator(OperandStack.Top(), RightHandOperand);
			};
			switch (Object.Value.Operator) {
			case '+':
				Operation([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand += RightHandOperand;
				});
				break;
			case '-':
				Operation([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand -= RightHandOperand;
				});
				break;
			case '*':
				Operation([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand *= RightHandOperand;
				});
				break;
			case '/':
				Operation([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand /= RightHandOperand;
				});
				break;
			default:
				break;
			}
		};
		for (auto &x : RPNExpression)
			if (!x.IsOperator)
				OperandStack += x.Value.Operand;
			else
				Evaluate(x);
		return OperandStack.Top();
	};
	auto PrintExpression = [](auto &Object) {
		for (auto &x : Object)
			std::cout << x << ' ';
		std::cout << std::endl;
	};
	std::cout << "Infix Expression: ";
	PrintExpression(InfixExpression);
	std::cout << "RPN Expression: ";
	GetRPNExpression();
	PrintExpression(RPNExpression);
	std::cout << "Result: " << EvaluateRPNExpression() << std::endl;
	system("pause");
}