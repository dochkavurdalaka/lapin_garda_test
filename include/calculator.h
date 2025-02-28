#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

using NumberType = std::variant<int64_t, double>;

class Tokenizer {
public:
    explicit Tokenizer(std::istream* in, std::unordered_map<std::string, NumberType>& database) : in_{in}, database_(database) {
        Consume();
    }

    enum TokenType { kNumber, kSymbol, kEnd };

    void Consume() {
        while (in_->peek() == ' ') {
            in_->get();
        }
        if (in_->peek() == std::char_traits<char>::eof()) {
            type_ = TokenType::kEnd;
        } else if (isdigit(in_->peek())) {
            std::string value;
            char inp = in_->peek();

            do {
                value += inp;
                in_->get();
                inp = in_->peek();
            } while (in_->peek() != std::char_traits<char>::eof() and (isdigit(inp) or inp == '.'));

            int count_dot = 0;
            for (auto el : value) {
                if (el == '.') {
                    count_dot += 1;
                }
            }
            if (count_dot == 1) {
                number_ = std::stod(value);
            } else if (count_dot == 0) {
                number_ = std::stoi(value);
            } else {
                throw std::invalid_argument("too many dots in number declaration");
            }
            type_ = TokenType::kNumber;
        } else if(isalpha(in_->peek())) {
            char inp = in_->peek();
            std::string variable_name;
            do {
                variable_name += inp;
                in_->get();
                inp = in_->peek();
            } while (in_->peek() != std::char_traits<char>::eof() and (std::isalpha(inp) or std::isdigit(inp)));
            if(database_.contains(variable_name)) {
                type_ = TokenType::kNumber;
                number_ = database_[variable_name];
            } else {
                throw std::invalid_argument("no variables with this name existing in database");
            }
        } else {
            *in_ >> symbol_;
            type_ = TokenType::kSymbol;
        }
    }

    TokenType GetType() const {
        return type_;
    }

    NumberType GetNumber() const {
        return number_;
    }

    char GetSymbol() const {
        return symbol_;
    }

private:
    std::istream* in_;

    std::unordered_map<std::string, NumberType>& database_;
    TokenType type_;
    NumberType number_;
    char symbol_;
};

NumberType operator*(const NumberType& one, const NumberType& two) {
    if (std::holds_alternative<int64_t>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<int64_t>(one) * std::get<int64_t>(two);
    } else if (std::holds_alternative<double>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<double>(one) * std::get<int64_t>(two);
    } else if (std::holds_alternative<int64_t>(one) and std::holds_alternative<double>(two)) {
        return std::get<int64_t>(one) * std::get<double>(two);
    }
    return std::get<double>(one) * std::get<double>(two);
}

NumberType operator+(const NumberType& one, const NumberType& two) {
    if (std::holds_alternative<int64_t>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<int64_t>(one) + std::get<int64_t>(two);
    } else if (std::holds_alternative<double>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<double>(one) + std::get<int64_t>(two);
    } else if (std::holds_alternative<int64_t>(one) and std::holds_alternative<double>(two)) {
        return std::get<int64_t>(one) + std::get<double>(two);
    }
    return std::get<double>(one) + std::get<double>(two);
}

NumberType operator-(const NumberType& one, const NumberType& two) {
    if (std::holds_alternative<int64_t>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<int64_t>(one) - std::get<int64_t>(two);
    } else if (std::holds_alternative<double>(one) and std::holds_alternative<int64_t>(two)) {
        return std::get<double>(one) - std::get<int64_t>(two);
    } else if (std::holds_alternative<int64_t>(one) and std::holds_alternative<double>(two)) {
        return std::get<int64_t>(one) - std::get<double>(two);
    }
    return std::get<double>(one) - std::get<double>(two);
}

NumberType operator/(const NumberType& one, const NumberType& two) {
    if (std::holds_alternative<int64_t>(one) and std::holds_alternative<int64_t>(two)) {
        if(std::get<int64_t>(two) == 0) {
            throw std::invalid_argument("division on zero is forbidden");
        }
        return std::get<int64_t>(one) / std::get<int64_t>(two);
    } else if (std::holds_alternative<double>(one) and std::holds_alternative<int64_t>(two)) {
        if(std::get<int64_t>(two) == 0) {
            throw std::invalid_argument("division on zero is forbidden");
        }
        return std::get<double>(one) / std::get<int64_t>(two);
    } else if (std::holds_alternative<int64_t>(one) and std::holds_alternative<double>(two)) {
        if(std::get<double>(two) == 0.) {
            throw std::invalid_argument("division on zero is forbidden");
        }
        return std::get<int64_t>(one) / std::get<double>(two);
    }
    if(std::get<double>(two) == 0.) {
        throw std::invalid_argument("division on zero is forbidden");
    }
    return std::get<double>(one) / std::get<double>(two);
}

class Expression {
public:
    virtual ~Expression() = default;
    virtual NumberType Evaluate() const = 0;
};

class Mult : public Expression {
public:
    Mult(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_{std::move(left)}, right_{std::move(right)} {
    }

    NumberType Evaluate() const override {
        return left_->Evaluate() * right_->Evaluate();
    }

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class Plus : public Expression {
public:
    Plus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_{std::move(left)}, right_{std::move(right)} {
    }

    NumberType Evaluate() const override {
        return left_->Evaluate() + right_->Evaluate();
    }

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class Minus : public Expression {
public:
    Minus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_{std::move(left)}, right_{std::move(right)} {
    }

    NumberType Evaluate() const override {
        return left_->Evaluate() - right_->Evaluate();
    }

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class Divide : public Expression {
public:
    Divide(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_{std::move(left)}, right_{std::move(right)} {
    }

    NumberType Evaluate() const override {
        return left_->Evaluate() / right_->Evaluate();
    }

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class UnaryMinus : public Expression {
public:
    UnaryMinus(std::unique_ptr<Expression> right) : right_{std::move(right)} {
    }

    NumberType Evaluate() const override {
        return NumberType(0) - right_->Evaluate();
    }

private:
    std::unique_ptr<Expression> right_;
};

class Number : public Expression {
public:
    explicit Number(const NumberType& number) : number_{number} {
    }

    NumberType Evaluate() const override {
        return number_;
    }

private:
    NumberType number_;
};

std::unique_ptr<Expression> ParseExpression(Tokenizer* tokenizer);

std::unique_ptr<Expression> ParseFactor(Tokenizer* tokenizer) {
    if (tokenizer->GetType() == Tokenizer::kSymbol and tokenizer->GetSymbol() == '-') {
        tokenizer->Consume();
        return std::make_unique<UnaryMinus>(ParseFactor(tokenizer));
    } else {
        if (tokenizer->GetType() == Tokenizer::kNumber) {
            auto number = std::make_unique<Number>(tokenizer->GetNumber());
            tokenizer->Consume();
            return number;
        } else {
            tokenizer->Consume();
            auto expr = ParseExpression(tokenizer);
            tokenizer->Consume();
            return expr;
        }
    }
}

std::unique_ptr<Expression> ParseTerm(Tokenizer* tokenizer) {
    std::unique_ptr<Expression> result = ParseFactor(tokenizer);
    char symbol_now;
    while ((tokenizer->GetType() == Tokenizer::kSymbol) &&
           ((symbol_now = tokenizer->GetSymbol()) == '*' or symbol_now == '/')) {
        tokenizer->Consume();
        if (symbol_now == '*') {
            result = std::make_unique<Mult>(std::move(result), ParseFactor(tokenizer));
        } else {
            result = std::make_unique<Divide>(std::move(result), ParseFactor(tokenizer));
        }
    }
    return result;
}

std::unique_ptr<Expression> ParseExpression(Tokenizer* tokenizer) {
    std::unique_ptr<Expression> result = ParseTerm(tokenizer);
    char symbol_now;
    while ((tokenizer->GetType() == Tokenizer::kSymbol) &&
           ((symbol_now = tokenizer->GetSymbol()) == '+' or symbol_now == '-')) {
        tokenizer->Consume();
        if (symbol_now == '+') {
            result = std::make_unique<Plus>(std::move(result), ParseTerm(tokenizer));
        } else {
            result = std::make_unique<Minus>(std::move(result), ParseTerm(tokenizer));
        }
    }
    return result;
}
