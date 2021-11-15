//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_RETURNSTATEMENT_H
#define NAIVECCOMPILER_RETURNSTATEMENT_H


#include "Statement.h"
#include "Expression.h"

class ReturnStatement : public Statement {
private:
    std::shared_ptr<const Expression> expression;
public:
    explicit ReturnStatement(decltype(expression) expression) : expression(std::move(expression)) {}

    void gen() const override {
        if (expression == nullptr) {
            Emitter::emit("return;");
            return;
        }
        Emitter::emit("return " + expression->rValue()->getString() + ";");
    }

    std::string getValue() const override {
        return "Return";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {expression};
    }
};


#endif //NAIVECCOMPILER_RETURNSTATEMENT_H
