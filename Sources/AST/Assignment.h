//
// Created by 魏宇翔 on 2020/11/26.
//

#ifndef NAIVECCOMPILER_ASSIGNMENT_H
#define NAIVECCOMPILER_ASSIGNMENT_H


#include <utility>
#include <memory>
#include "Identifier.h"
#include "Expression.h"
#include "Statement.h"
#include "Emitter.h"

/// id = expr;
class Assignment : public Statement {
private:
    std::shared_ptr<const Identifier> id;
    std::shared_ptr<const Expression> expression;
public:
    Assignment(decltype(id) id, decltype(expression) expression) :
            id(std::move(id)), expression(std::move(expression)) {}

    void gen() const override {
        Emitter::emit(id->rValue()->getString() + " = " + expression->rValue()->getString());
    }

    std::string getValue() const override {
        return "=";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {id, expression};
    }
};


#endif //NAIVECCOMPILER_ASSIGNMENT_H
