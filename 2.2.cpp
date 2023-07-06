#include <iostream>
#include <string>
#include <stack>

// Function to evaluate binary mathematical expressions
int evaluateExpression(const std::string& expression) {
    std::stack<int> operandStack;
    std::stack<char> operatorStack;

    auto precedence = [](char op) {
        if (op == '*' || op == '/')
            return 2;
        else if (op == '+' || op == '-')
            return 1;
        else
            return 0;
    };

    auto applyOperator = [&]() {
        int operand2 = operandStack.top();
        operandStack.pop();
        int operand1 = operandStack.top();
        operandStack.pop();
        char oper = operatorStack.top();
        operatorStack.pop();
        int result;
        switch (oper) {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            if (operand2 == 0) {
                throw std::runtime_error("Error: Division by zero");
            }
            result = operand1 / operand2;
            break;
        default:
            throw std::runtime_error("Syntax error");
        }
        operandStack.push(result);
    };

    for (size_t i = 0; i < expression.length(); ++i) {
        if (isdigit(expression[i])) {
            int num = expression[i] - '0';
            while (i + 1 < expression.length() && isdigit(expression[i + 1])) {
                i++;
                num = num * 10 + (expression[i] - '0');
            }
            operandStack.push(num);
        }
        else if (expression[i] == '(') {
            operatorStack.push(expression[i]);
        }
        else if (expression[i] == ')') {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                applyOperator();
            }
            if (!operatorStack.empty() && operatorStack.top() == '(') {
                operatorStack.pop();
            }
            else {
                throw std::runtime_error("Syntax error");
            }
        }
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            if (i == 0 || (!isdigit(expression[i - 1]) && expression[i - 1] != ')')) {
                // Handle negative numbers
                if (expression[i] == '-') {
                    int num = 0;
                    while (i + 1 < expression.length() && isdigit(expression[i + 1])) {
                        i++;
                        num = num * 10 + (expression[i] - '0');
                    }
                    num = -num;
                    operandStack.push(num);
                }
                else {
                    throw std::runtime_error("Syntax error");
                }
            }
            else {
                while (!operatorStack.empty() && precedence(expression[i]) <= precedence(operatorStack.top())) {
                    applyOperator();
                }
                operatorStack.push(expression[i]);
            }
        }
        else if (expression[i] != ' ') {
            throw std::runtime_error("Syntax error");
        }
    }

    while (!operatorStack.empty()) {
        applyOperator();
    }

    if (operandStack.size() != 1 || !operatorStack.empty()) {
        throw std::runtime_error("Syntax error");
    }

    return operandStack.top();
}

int main() {
    std::string input;
    std::cout << "Enter desired mathematical expressions with operators '+' '-' '*' '/' and parentheses '(' ')'. Enter 'Quit' to terminate input:\n";

    std::cout << "> ";
    while (std::getline(std::cin, input) && input != "Quit") {
        try {
            int result = evaluateExpression(input);
            std::cout << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        std::cout << "> ";
    }

    return 0;
}
