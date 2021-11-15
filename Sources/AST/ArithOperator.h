//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_ARITHOPERATOR_H
#define NAIVECCOMPILER_ARITHOPERATOR_H


#include "Expression.h"
#include "Emitter.h"
#include "Temp.h"
#include <string>
#include <utility>
#include <memory>

/// + - * /
class ArithOperator : public Expression {
public:
    enum class Kind {
        plus, minus, mult, div
    };
private:
    Kind opKind;
    std::shared_ptr<const Expression> lhs;
    std::shared_ptr<const Expression> rhs;
public:
    ArithOperator(decltype(lhs) lhs, Kind opKind, decltype(rhs) rhs) :
            lhs(std::move(lhs)), opKind(opKind), rhs(std::move(rhs)) {}

    std::shared_ptr<const Expression> lValue() const override {
        return nullptr;
    }

    std::shared_ptr<const Expression> rValue() const override {
        const auto temp = std::make_shared<Temp>();
        Emitter::emit(temp->getString() + " = " + lhs->rValue()->getString() + " " +
                      getOpString() + " " + rhs->rValue()->getString());
        return temp;
    }

    std::string getString() const override {
        return lhs->getString() + " " + getOpString() + " " + rhs->getString();
    }

    static Kind getKindFromToken(const Token &token) {
        if (token.lexeme == "+") {
            return Kind::plus;
        } else if (token.lexeme == "-") {
            return Kind::minus;
        } else if (token.lexeme == "*") {
            return Kind::mult;
        } else if (token.lexeme == "/") {
            return Kind::div;
        } else {
            throw std::runtime_error("Lexeme '" + token.lexeme + "' should not occur here.");
        }
    }

    std::string getValue() const override {
        return getOpString();
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override{
        return {lhs, rhs};
    }

private:
    std::string getOpString() const {

        std::string opString;
        switch (opKind) {
            case Kind::plus:
                opString = "+";
                break;
            case Kind::minus:
                opString = "-";
                break;
            case Kind::mult:
                opString = "*";
                break;
            case Kind::div:
                opString = "/";
                break;
        }
        return opString;
    }
};


#endif //NAIVECCOMPILER_ARITHOPERATOR_H
