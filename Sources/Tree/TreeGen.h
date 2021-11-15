//
// Created by 魏宇翔 on 2020/11/30.
//

#ifndef NAIVECCOMPILER_TREEGEN_H
#define NAIVECCOMPILER_TREEGEN_H


#include "Tree.h"
#include "../Utilities/FilterNull.h"
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

// /// 产生一个唯一ID
// struct IDGenerator {
// private:
//     static int id;
// public:
//     static int getNewID() {
//         return ++id;
//     }
//
//     IDGenerator() = delete;
// };
//
// int IDGenerator::id = 0;

/// 生成树的dot文件格式，随后用graphviz画图，用指针值作为id
template<typename TreeNode, typename GetChildren, typename MapFunc>
void generateTreeFromNode(std::ostream &os, TreeNode treeNode, GetChildren getChildren, MapFunc mapFunc/*,
                  std::function<int(std::shared_ptr<TreeNode<Value>>)> idFunc =
                  [](std::shared_ptr<TreeNode<Value>> ptr) -> int { return ptr; }*/) {
    const auto &children = filterNull(getChildren(treeNode));
    for (const auto &child : children) {
        generateTreeFromNode(os, child, getChildren, mapFunc);
    }

    // GraphvizNode?[label="xxx"]
    os << "Node" << treeNode << "[label=\"" << mapFunc(treeNode) << "\"];\n";
    // GraphvizNode? -> GraphvizNode*
    for (const auto &child : children) {
        os << "Node" << treeNode << " -> " << "Node" << child << ";\n";
    }
    os << '\n';
}

template<typename Tree, typename GetRoot, typename GetChildren, typename MapFunc>
void generateTree(std::ostream &os, Tree tree, GetRoot getRoot, GetChildren getChildren, MapFunc mapFunc) {
    os << "digraph ParseTree {\n/*edge[color=white];node[style=filled,color=white]\n*/";
    generateTreeFromNode(os, getRoot(tree), getChildren, mapFunc);
    os << "}" << std::endl;
}

#endif //NAIVECCOMPILER_TREEGEN_H
