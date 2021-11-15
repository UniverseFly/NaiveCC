//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_NULLSTATEMENT_H
#define NAIVECCOMPILER_NULLSTATEMENT_H


#include "Statement.h"

class NullStatement : public Statement {
private:
    void gen() const override {}

public:
    static std::shared_ptr<const NullStatement> init() {
        return std::make_shared<const NullStatement>();
    }

    std::string getValue() const override {
        return "";
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override {
        return {};
    }
};


#endif //NAIVECCOMPILER_NULLSTATEMENT_H
