//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_DFABUILDER_H
#define NAIVECCOMPILER_DFABUILDER_H


#include "../Lex/Token.h"
#include <unordered_set>
#include <map>
#include <set>
#include <variant>
#include "../Utilities/VariantVisitor.h"
#include "Grammar.h"
#include "LR1Item.h"

/// 用来生成LR1的项目集DFA
class DFABuilder {
private:
    /// 原始文法
    const Grammar augmentedGrammar;

    std::vector<std::unordered_set<LR1Item>> canonicalCollection;
    std::unordered_map<int, std::unordered_map<int, int>> transitions;
public:
    // /// 打印canonicalCollection来debug
    // void printStates() const {
    //     int id = 0;
    //     for (const auto &items : canonicalCollection) {
    //         std::cout << "\n\n目前是状态" << id << std::endl;
    //         int count = 0;
    //         for (const auto &item : items) {
    //             if (count % 10 == 0) { std::cout << std::endl; }
    //             std::printf("[]   ", pr)
    //         }
    //         ++id;
    //     }
    // }

    /// 要求直接提供augmented grammar
    explicit DFABuilder(Grammar augmentedGrammar) : augmentedGrammar(std::move(augmentedGrammar)) {
        buildCanonicalCollection();
    }

    // Ref: eac2 english. ver. p130
    void buildCanonicalCollection() {
        // S' -> •S 的表示，一定是第一个产生式对应的
        const LR1Item startItem(0, 0, augmentedGrammar.terminatorIndex);
        /// 初始状态
        canonicalCollection.push_back(closure({startItem}));
        /// 记录是否处理过
        std::unordered_map<int, bool> isProcessed;
        while (true) {
            /// 记录先前的CC的size
            const auto previousSize = canonicalCollection.size();
            for (int i = 0; i < canonicalCollection.size(); ++i) {
                // 处理过了continue
                if (isProcessed.find(i) != isProcessed.cend() && isProcessed.at(i)) { continue; }
                isProcessed.insert({i, true});
                for (const auto &item : canonicalCollection[i]) {
                    // •x...的时候工作，如果是x•就不会有transition
                    if (item.isDotWithinIndices(augmentedGrammar)) {
                        // select the x following •
                        // 也就是说只有•之后的symbol才会被添加到转移函数
                        const auto firstSymbol = item.firstSymbolAfterDot(augmentedGrammar);
                        // goto(current, x)
                        const auto newState = stateGoto(canonicalCollection[i], firstSymbol);
                        // 检查是不是在CC里面
                        bool isAlreadyIn = false;
                        /// 找到的已有状态下标
                        int j = 0;
                        for (; j < canonicalCollection.size(); ++j) {
                            if (canonicalCollection[j] == newState) {
                                isAlreadyIn = true;
                                break;
                            }
                        }
                        // 添加转移函数
                        if (transitions.find(i) == transitions.cend()) {
                            transitions.insert({i, std::unordered_map<int, int>()});
                        }
                        if (!isAlreadyIn) {
                            transitions.at(i).insert({firstSymbol, canonicalCollection.size()});
                            canonicalCollection.push_back(newState);
                        } else {
                            transitions.at(i).insert({firstSymbol, j});
                        }
                    }
                }
            }
            // 如果 size 不变就结束
            if (previousSize == canonicalCollection.size()) {
                break;
            }
        }
    }

    const Grammar &getAugmentedGrammar() const {
        return augmentedGrammar;
    }

    const std::vector<std::unordered_set<LR1Item>> &getCanonicalCollection() const {
        return canonicalCollection;
    }

    const std::unordered_map<int, std::unordered_map<int, int>> &getTransitions() const {
        return transitions;
    }

private:
    /// Goto function, which indicates state transitions
    std::unordered_set<LR1Item>
    stateGoto(const std::unordered_set<LR1Item> &items, int symbol) {
        std::unordered_set<LR1Item> result;
        for (const auto &item : items) {
            // 如果有 A -> a•Xb, c，就add A -> aX•b, c
            if (item.isDotWithinIndices(augmentedGrammar) &&
                item.firstSymbolAfterDot(augmentedGrammar) == symbol) {
                result.insert({item.productionIndex, item.dotIndex + 1, item.lookaheadIndex});
            }
        }
        return closure(result);
    }

    /// closure of a set of lr1 items
    /// referred to EAC2 English Ver. P128
    std::unordered_set<LR1Item> closure(const std::unordered_set<LR1Item> &items) {
        auto result = items;
        bool isChanged = true;
        while (isChanged) {
            isChanged = false;
            /// used to check if the result is changed
            const auto previousSize = result.size();
            for (const auto &item : result) {
                if (item.isDotWithinIndices(augmentedGrammar)) {
                    const auto &firstSymbol = item.firstSymbolAfterDot(augmentedGrammar);
                    // do nothing if it's a terminal
                    if (firstSymbol >= 0) {
                        /// A -> β•Cδ,a 里的 δ,a
                        const auto &remaining = item.remaining(augmentedGrammar);
                        // productions with left part being `firstSymbol`
                        const auto &productions = augmentedGrammar.productionsOfNonTerminal.at(firstSymbol);
                        for (const auto symbol : augmentedGrammar.first(remaining)) {
                            for (const auto &productionIndex : productions) {
                                result.insert({productionIndex, 0, symbol});
                            }
                        }
                    }
                }
            }
            if (previousSize != result.size()) {
                isChanged = true;
            }
        }
        return result;
    }

    // static Grammar getAugmentedGrammar(Grammar &&grammar) {
    //     auto productions = grammar.productions;
    //     auto nonTerminals = grammar.nonTerminals;
    //     // S'
    //     nonTerminals.push_back(nonTerminals[0] + "'");
    //     // S' -> S
    //     productions.push_back({nonTerminals.size() - 1, {0}});
    //     return {nonTerminals, grammar.terminals, productions};
    // }
};


#endif //NAIVECCOMPILER_DFABUILDER_H
