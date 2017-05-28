#include <iostream>
#include "Stack.hpp"
#include "List.hpp"

struct ExpressionNode final {
private:
	union DataSet final {
		decltype(0.) Operand = 0.;
		decltype('0') Operator;
		DataSet() = default;
		DataSet(double Operand) {
			this->Operand = Operand;
		}
		DataSet(char Operator) {
			this->Operator = Operator;
		}
		DataSet(const DataSet &) = default;
		DataSet(DataSet &&) = default;
		auto operator=(const DataSet &)->DataSet & = default;
		auto operator=(DataSet &&)->DataSet & = default;
		~DataSet() = default;
	};
public:
	DataSet DataUnion = {};
	decltype(0) Precedence = 0;
	decltype(false) IsOperator = false;
	ExpressionNode() = default;
	ExpressionNode(double Operand) {
		DataUnion = Operand;
	}
	ExpressionNode(char Operator) {
		auto GetPrecedence = [&]() {
			switch (Operator) {
			case '+': case '-':
				return 1;
			case '*': case '/':
				return 2;
			default:
				return 0;
			}
		};
		DataUnion = Operator;
		IsOperator = true;
		Precedence = GetPrecedence();
	}
	ExpressionNode(const ExpressionNode &) = default;
	ExpressionNode(ExpressionNode &&) = default;
	auto operator=(const ExpressionNode &)->ExpressionNode & = default;
	auto operator=(ExpressionNode &&)->ExpressionNode & = default;
	~ExpressionNode() = default;
	auto operator==(char Operator) const {
		return IsOperator && DataUnion.Operator == Operator;
	}
	auto operator!=(char Operator) const {
		return !(*this == Operator);
	}
	friend auto &operator<<(std::ostream &Output, const ExpressionNode &SomeExpressionNode) {
		if (SomeExpressionNode.IsOperator)
			Output << SomeExpressionNode.DataUnion.Operator;
		else
			Output << SomeExpressionNode.DataUnion.Operand;
		return Output;
	}
};

auto main()->int {
	using Expression = StandardTemplateLibrary::List<ExpressionNode>;
	auto InfixExpression = Expression{ '(', 12., '-', 8., ')', '/', 4., '+', 2., '*', 3. };
	auto RPNExpression = Expression{};
	auto GetRPNExpression = [&]() {
		using ExpressionStack = StandardTemplateLibrary::Stack<ExpressionNode, Expression>;
		auto OperatorStack = ExpressionStack{ '#' };
		auto PopToRPNExpression = [&]() {
			RPNExpression += OperatorStack.Top();
			--OperatorStack;
		};
		auto OperandToRPNExpression = [&](auto &Operand) {
			RPNExpression += Operand;
		};
		auto OperatorToRPNExpression = [&](auto &Operator) {
			if (Operator == '(')
				OperatorStack += Operator;
			else if (Operator == ')') {
				while (OperatorStack.Top() != '(')
					PopToRPNExpression();
				--OperatorStack;
			}
			else
				if (Operator.Precedence > OperatorStack.Top().Precedence)
					OperatorStack += Operator;
				else {
					while (OperatorStack.Size() > 1 && Operator.Precedence <= OperatorStack.Top().Precedence)
						PopToRPNExpression();
					OperatorStack += Operator;
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
		auto Evaluate = [&](auto Operator) {
			auto TakeAction = [&](auto &&Action) {
				auto RightHandOperand = OperandStack.Top();
				--OperandStack;
				Action(OperandStack.Top(), RightHandOperand);
			};
			switch (Operator) {
			case '+':
				TakeAction([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand += RightHandOperand;
				});
				break;
			case '-':
				TakeAction([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand -= RightHandOperand;
				});
				break;
			case '*':
				TakeAction([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand *= RightHandOperand;
				});
				break;
			case '/':
				TakeAction([](auto &LeftHandOperand, auto RightHandOperand) {
					LeftHandOperand /= RightHandOperand;
				});
				break;
			default:
				break;
			}
		};
		for (auto &x : RPNExpression)
			if (!x.IsOperator)
				OperandStack += x.DataUnion.Operand;
			else
				Evaluate(x.DataUnion.Operator);
		return OperandStack.Top();
	};
	auto PrintExpression = [](auto &Expression) {
		for (auto &x : Expression)
			std::cout << x << ' ';
		std::cout << std::endl;
	};
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "Infix Expression: ";
	PrintExpression(InfixExpression);
	std::cout << "RPN Expression: ";
	GetRPNExpression();
	PrintExpression(RPNExpression);
	std::cout << "Result: " << EvaluateRPNExpression() << std::endl;
	Pause();
}