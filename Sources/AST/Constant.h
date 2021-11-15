//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_CONSTANT_H
#define NAIVECCOMPILER_CONSTANT_H


#include "Expression.h"
#include <sstream>

/// 代表字面值常量
class Constant : public Expression {
public:
    explicit Constant(int value) : value(value) {}

    std::shared_ptr<const Expression> lValue() const override {
        return std::make_shared<const Constant>(*this);
    }

    std::shared_ptr<const Expression> rValue() const override {
        return std::make_shared<const Constant>(*this);
    }

    std::string getString() const override {
        return [this]() -> std::string {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }();
    }

    static int getValueFromToken(const Token &token) {
        return std::atoi(token.getLexeme().c_str());
    }

    std::string getValue() const override {
        return std::to_string(value);
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {};
    }

private:
    int value;
};


#endif //NAIVECCOMPILER_CONSTANT_H
