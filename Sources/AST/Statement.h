//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_STATEMENT_H
#define NAIVECCOMPILER_STATEMENT_H


#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include "GraphvizNode.h"

/// 语句在 AST 中的抽象接口
struct Statement : GraphvizNode {
    /// 产生三地址码
    virtual void gen() const = 0;

    static std::string newLabel() {
        std::ostringstream oss;
        oss << "L" << labelCount++;
        return oss.str();
    }

    virtual ~Statement() = default;

private:
    static unsigned labelCount;
};

unsigned Statement::labelCount = 0;


#endif //NAIVECCOMPILER_STATEMENT_H
