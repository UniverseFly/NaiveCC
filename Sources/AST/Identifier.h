//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_IDENTIFIER_H
#define NAIVECCOMPILER_IDENTIFIER_H


#include <string>
#include <memory>
#include "Expression.h"

class Identifier : public Expression {
private:
    std::string value;
public:
    explicit Identifier(std::string value) : value(std::move(value)) {}

    std::shared_ptr<const Expression> lValue() const override {
        return std::make_shared<const Identifier>(*this);
    }

    std::shared_ptr<const Expression> rValue() const override {
        return std::make_shared<const Identifier>(*this);
    }

    std::string getString() const override { return value; }

    std::string getValue() const override {
        return value;
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {};
    }
};


#endif //NAIVECCOMPILER_IDENTIFIER_H
