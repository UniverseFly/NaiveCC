//
// Created by 魏宇翔 on 2020/11/30.
//

#ifndef NAIVECCOMPILER_GRAPHVIZNODE_H
#define NAIVECCOMPILER_GRAPHVIZNODE_H


#include <string>

/// 注意，这个node只是用来生成dot文件，和AST本身没关系
struct GraphvizNode {
    virtual std::string getValue() const = 0;

    virtual std::vector<std::shared_ptr<const GraphvizNode>> getChildren() const = 0;

    virtual ~GraphvizNode() = default;
};


#endif //NAIVECCOMPILER_GRAPHVIZNODE_H
