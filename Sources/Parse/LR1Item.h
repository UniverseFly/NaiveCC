//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_LR1ITEM_H
#define NAIVECCOMPILER_LR1ITEM_H


#include <utility>
#include <vector>
#include "../Lex/Token.h"
#include "Grammar.h"

class LR1Item {
public:
    /// 记录原始产生式的下标，这样做更利于reduce时候找到下标
    const int productionIndex;
    /// 产生式里•的位置 [0, production.rightSide.length]
    const int dotIndex;
    /// lookahead 在 grammar.terminals里的index
    const int lookaheadIndex;
public:
    LR1Item(int productionIndex,
            int dotIndex, int lookaheadIndex) :
            productionIndex(productionIndex), dotIndex(dotIndex), lookaheadIndex(lookaheadIndex) {}

    int firstSymbolAfterDot(const Grammar &grammar) const {
        return grammar.productions[productionIndex].second[dotIndex];
    }

    bool isDotWithinIndices(const Grammar &grammar) const {
        return dotIndex < grammar.productions[productionIndex].second.size();
    }

    /// A -> β•Cδ,a 里的 δ,a
    std::vector<int> remaining(const Grammar &grammar) const {
        const auto &production = grammar.productions[productionIndex];
        std::vector<int> result;
        for (int i = dotIndex + 1; i < production.second.size(); ++i) {
            result.push_back(production.second[i]);
        }
        result.push_back(lookaheadIndex);
        return result;
    }

    bool operator==(const LR1Item &rhs) const;
};

namespace std {
    template<>
    struct hash<LR1Item> {
        std::size_t operator()(const LR1Item &item) const {
            return hash<int>()(item.productionIndex) ^ hash<int>()(item.dotIndex) ^ hash<int>()(item.lookaheadIndex);
        }
    };
}

bool LR1Item::operator==(const LR1Item &rhs) const {
    return productionIndex == rhs.productionIndex &&
           dotIndex == rhs.dotIndex &&
           lookaheadIndex == rhs.lookaheadIndex;
}

#endif //NAIVECCOMPILER_LR1ITEM_H
