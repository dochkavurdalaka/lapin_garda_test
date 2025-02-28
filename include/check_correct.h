#include <string>
#include <stdexcept>

bool IsMulOrDiv(char c) {
    if (c == '*' or c == '/') {
        return true;
    }
    return false;
}

bool IsOperation(char op) {
    if (op == '+' || op == '-' || op == '*' || op == '/') {
        return true;
    }
    return false;
}

void ExamOnInCorrect(const std::string& expr) {
    int count = 0;
    for (size_t i = 0; i < expr.size(); i++) {
        if (expr[i] == '(') {
            count++;
        } else if (expr[i] == ')') {
            count--;
        }
        if (count < 0) {
            throw std::invalid_argument("number of closing brackets is bigger then number of opening");
        }
    }
    if (count != 0) {
        throw std::invalid_argument("num of closing and opening brackets not equal");
    }

    for (size_t i = 0; i < expr.size(); i++) {
        if (i == 0) {
            if (IsMulOrDiv(expr[i])) {
                throw std::invalid_argument(
                    "multiplication or dividing operation cannot be first symbol of equation");
            }
        } else {
            if (IsMulOrDiv(expr[i]) and IsOperation(expr[i - 1])) {
                throw std::invalid_argument("binary operations before multiplying or dividing");
            } else if (IsMulOrDiv(expr[i]) and expr[i - 1] == '(') {
                throw std::invalid_argument("binary operation after opening bracket");
            } else if (expr[i] == ')' and IsOperation(expr[i - 1])) {
                throw std::invalid_argument("operation before closing bracket");
            } else if ((isdigit(expr[i]) or isalpha(expr[i])) and expr[i - 1] == ')') {
                throw std::invalid_argument("symbol or digit after closing bracket");
            } else if (expr[i] == '(' and (isalpha(expr[i - 1]) or isdigit(expr[i - 1]))) {
                throw std::invalid_argument("symbol or alpha before opening bracket");
            } else if (expr[i] == '.' and (not isdigit(expr[i - 1]))) {
                throw std::invalid_argument("dot must be next after digit");
            } 
        }
    }
    if(IsOperation(expr.back())) {
        throw std::invalid_argument("last symbol can't be operation");
    }
}