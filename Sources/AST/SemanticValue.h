//
// Created by 魏宇翔 on 2020/11/27.
//

#ifndef NAIVECCOMPILER_SEMANTICVALUE_H
#define NAIVECCOMPILER_SEMANTICVALUE_H


#include "Statement.h"
#include "Expression.h"
#include <variant>
#include <memory>

using SemanticValue = std::variant<
        std::shared_ptr<const Statement>,
        std::shared_ptr<const Expression>,
        const Token
        // // 函数调用的argList，这个不是很好..有点违背设计原则。
        // std::pair<std::shared_ptr<const Expression>>
>;


#endif //NAIVECCOMPILER_SEMANTICVALUE_H
