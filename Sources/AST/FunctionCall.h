//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_FUNCTIONCALL_H
#define NAIVECCOMPILER_FUNCTIONCALL_H


#include "Expression.h"
#include "Identifier.h"
#include "Temp.h"
#include "Emitter.h"
#include <vector>

class FunctionCall : public Expression {
private:
    std::shared_ptr<const Identifier> id;
    std::vector<std::shared_ptr<const Expression>> args;
public:
    FunctionCall(decltype(id) id, decltype(args) args) :
            id(std::move(id)), args(std::move(args)) {}

    std::shared_ptr<const Expression> lValue() const override {
        return nullptr;
    }

    std::shared_ptr<const Expression> rValue() const override {
        std::string argString;
        for (auto ptr = args.cbegin(); ptr != args.cend(); ++ptr) {
            argString += (*ptr)->rValue()->getString();
            if (ptr != args.cend() - 1) {
                argString += ", ";
            }
        }
        auto temp = std::make_shared<Temp>();
        Emitter::emit(temp->getString() + " = " + id->getString() + "(" + argString + ")");
        return temp;
    }

    std::string getString() const override {
        throw std::runtime_error("Unimplemented");
        return {};
    }

    std::string getValue() const override {
        throw std::runtime_error("Unimplemented");
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override{
        throw std::runtime_error("Unimplemented");
    }
};


#endif //NAIVECCOMPILER_FUNCTIONCALL_H
