//
// Created by 魏宇翔 on 2020/11/27.
//

#ifndef NAIVECCOMPILER_STACKSTATE_H
#define NAIVECCOMPILER_STACKSTATE_H


#include "../AST/SemanticValue.h"
#include "../Tree/Tree.h"

class StackState {
public:
    using NodeValue = std::variant<int, Token>;
public:
    const int symbol;
    const int state;
    /// AST 的节点
    const SemanticValue astNode;
    /// 语法分析树节点，用索引，int是非终结符、epsilon、terminator，Token就是token
    const std::shared_ptr<TreeNode<NodeValue>> parseTreeNode;
public:
    StackState(int symbol, int state, SemanticValue astNode, std::shared_ptr<TreeNode<NodeValue>> parseTreeNode) :
            symbol(symbol), state(state), astNode(std::move(astNode)), parseTreeNode(std::move(parseTreeNode)) {}
};


#endif //NAIVECCOMPILER_STACKSTATE_H
