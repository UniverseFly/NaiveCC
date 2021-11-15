//
// Created by 魏宇翔 on 2020/11/30.
//

#ifndef NAIVECCOMPILER_TREE_H
#define NAIVECCOMPILER_TREE_H


#include <memory>
#include <deque>

/// 一颗树，直接把children放在vector里了，不能用deque，会报TreeNode是Incomplete type的错
template<typename Value>
class TreeNode {
private:
    Value value;
    std::deque<std::shared_ptr<TreeNode>> children;
public:
    explicit TreeNode(Value &&value, std::deque<std::shared_ptr<TreeNode>> children = {}) :
            value(std::forward<Value>(value)), children(std::move(children)) {}

    explicit TreeNode(const Value &value, std::deque<std::shared_ptr<TreeNode>> children = {}) :
            value(value), children(std::move(children)) {}

    const Value &getValue() const {
        return value;
    }

    const std::deque<std::shared_ptr<TreeNode>> &getChildren() const {
        return children;
    }
};

template<typename Value>
class Tree {
private:
    const std::shared_ptr<TreeNode<Value>> root;
public:
    explicit Tree(std::shared_ptr<TreeNode<Value>> root) : root(std::move(root)) {}

    const std::shared_ptr<TreeNode<Value>> &getRoot() const {
        return root;
    }
};

#endif //NAIVECCOMPILER_TREE_H
