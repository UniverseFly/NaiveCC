//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_RELATIONOPERATOR_H
#define NAIVECCOMPILER_RELATIONOPERATOR_H


#include "Expression.h"
#include "Temp.h"
#include "Emitter.h"
#include <memory>

class RelationOperator : public Expression {
public:
    enum class Kind {
        le, lt, ge, gt, eq, neq
    };
private:
    std::shared_ptr<const Expression> lhs;
    std::shared_ptr<const Expression> rhs;
    Kind opKind;
public:
    RelationOperator(decltype(lhs) lhs, Kind opKind, decltype(rhs) rhs) :
            lhs(std::move(lhs)), opKind(opKind), rhs(std::move(rhs)) {}

    std::shared_ptr<const Expression> lValue() const override {
        return nullptr;
    }

    std::shared_ptr<const Expression> rValue() const override {
        auto temp = std::make_shared<Temp>();
        Emitter::emit(temp->getString() + " = " + lhs->rValue()->getString() + " " +
                      getOpString() + " " + rhs->rValue()->getString());
        return temp;
    }

    std::string getString() const override {
        return lhs->getString() + " " + getOpString() + " " + rhs->getString();
    }

    std::string getOpString() const {
        std::string opString;
        switch (opKind) {
            case Kind::lt:
                opString = "<";
                break;
            case Kind::le:
                opString = "<=";
                break;
            case Kind::gt:
                opString = ">";
                break;
            case Kind::ge:
                opString = ">=";
                break;
            case Kind::eq:
                opString = "=";
                break;
            case Kind::neq:
                opString = "!=";
                break;
        }
        return opString;
    }

    static Kind getKindFromToken(const Token &token) {
        if (token.lexeme == "==") {
            return Kind::eq;
        } else if (token.lexeme == "<=") {
            return Kind::le;
        } else if (token.lexeme == ">=") {
            return Kind::ge;
        } else if (token.lexeme == "!=") {
            return Kind::neq;
        } else if (token.lexeme == "<") {
            return Kind::lt;
        } else if (token.lexeme == ">") {
            return Kind::gt;
        } else {
            throw std::runtime_error("Lexeme '" + token.lexeme + "' should not occur here.");
        }
    }

    std::string getValue() const override {
        return getOpString();
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {lhs, rhs};
    }
};


#endif //NAIVECCOMPILER_RELATIONOPERATOR_H
