//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_IFELSESTATEMENT_H
#define NAIVECCOMPILER_IFELSESTATEMENT_H


#include "Expression.h"
#include "Statement.h"
#include "Temp.h"
#include "Emitter.h"

class IfElseStatement : public Statement {
private:
    std::shared_ptr<const Expression> condition;
    std::shared_ptr<const Statement> statement1;
    std::shared_ptr<const Statement> statement2;
public:
    IfElseStatement(decltype(condition) condition,
                    decltype(statement1) statement1,
                    decltype(statement2) statement2) :
            condition(std::move(condition)), statement1(std::move(statement1)), statement2(std::move(statement2)) {}

    void gen() const override {
        auto label1 = newLabel();
        Emitter::emit("ifFalse " + condition->rValue()->getString() + " goto " + label1);

        statement1->gen();
        auto label2 = newLabel();
        Emitter::emit("goto " + label2);

        Emitter::emit(label1 + ":");
        statement2->gen();

        Emitter::emit(label2 + ":");
    }

    std::string getValue() const override {
        return "If-Else";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {condition, statement1, statement2};
    }
};


#endif //NAIVECCOMPILER_IFELSESTATEMENT_H
