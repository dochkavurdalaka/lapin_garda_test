#pragma once

#include "calculator.h"

#include <string>
#include <sstream>

NumberType EvaluateExpression(std::string expression, std::unordered_map<std::string, NumberType>& database) {
    std::istringstream ss{std::move(expression)};
    Tokenizer tokenizer{&ss, database};
    auto expr = ParseExpression(&tokenizer);
    return expr->Evaluate();
}
