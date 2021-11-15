//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_EXPRESSION_H
#define NAIVECCOMPILER_EXPRESSION_H


#include <memory>
#include <string>
#include "GraphvizNode.h"

/// 表达式的中间代码抽象接口类
struct Expression : GraphvizNode {
    virtual std::shared_ptr<const Expression> rValue() const = 0;

    virtual std::shared_ptr<const Expression> lValue() const = 0;

    virtual std::string getString() const = 0;

    virtual ~Expression() = default;
};


#endif //NAIVECCOMPILER_EXPRESSION_H
