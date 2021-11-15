//
// Created by 魏宇翔 on 2020/11/26.
//

#ifndef NAIVECCOMPILER_WHILESTATEMENT_H
#define NAIVECCOMPILER_WHILESTATEMENT_H


#include "Statement.h"
#include "Expression.h"
#include "Emitter.h"

class WhileStatement : public Statement {
private:
    std::shared_ptr<const Expression> condition;
    std::shared_ptr<const Statement> statement;
public:
    WhileStatement(decltype(condition) condition, decltype(statement) statement) :
            condition(std::move(condition)), statement(std::move(statement)) {}

    void gen() const override {
        auto label1 = newLabel();
        Emitter::emit(label1 + ":");

        /// 循环结束后的 label
        auto label2 = newLabel();
        Emitter::emit("ifFalse " + condition->rValue()->getString() + " goto " + label2);

        statement->gen();
        Emitter::emit("goto " + label1);

        Emitter::emit(label2 + ":");
    }

    std::string getValue() const override {
        return "While";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {condition, statement};
    }
};


#endif //NAIVECCOMPILER_WHILESTATEMENT_H
