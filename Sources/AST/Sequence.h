//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_SEQUENCE_H
#define NAIVECCOMPILER_SEQUENCE_H


#include "Statement.h"

/// 语句序列
class Sequence : public Statement {
private:
    std::shared_ptr<const Statement> statement;
    std::shared_ptr<const Statement> rest;
public:
    Sequence(decltype(statement) statement, decltype(rest) rest) :
            statement(std::move(statement)), rest(std::move(rest)) {}

    /// 产生三地址码
    void gen() const override {
        statement->gen();
        if (rest == nullptr) { return; }
        rest->gen();
    }

    std::string getValue() const override {
        return "Sequence";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {statement, rest};
    }
};


#endif //NAIVECCOMPILER_SEQUENCE_H
