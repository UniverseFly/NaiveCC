//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_IFSTATEMENT_H
#define NAIVECCOMPILER_IFSTATEMENT_H


#include "Statement.h"
#include "Emitter.h"
#include "Expression.h"

class IfStatement : public Statement {
private:
    std::shared_ptr<const Expression> expression;
    std::shared_ptr<const Statement> statement;
public:
    IfStatement(decltype(expression) expression, decltype(statement) statement) :
            expression(std::move(expression)), statement(std::move(statement)) {}

    IfStatement() = default;

    void gen() const override {
        std::string label(newLabel());
        Emitter::emit("ifFalse " + expression->rValue()->getString() + " goto " + label);
        statement->gen();
        Emitter::emit(label + ":");
    }

    std::string getValue() const override {
        return "If";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {expression, statement};
    }
};


#endif //NAIVECCOMPILER_IFSTATEMENT_H
