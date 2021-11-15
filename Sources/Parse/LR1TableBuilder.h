//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_LR1TABLEBUILDER_H
#define NAIVECCOMPILER_LR1TABLEBUILDER_H


#include "../Lex/Token.h"
#include <vector>
#include <unordered_map>
#include <exception>
#include "Action.h"
#include "Grammar.h"
#include "LR1Item.h"

/// LR1 分析表产生器
class LR1TableBuilder {
public:
    // Action[state, terminal] = ...action
    using ActionTable = std::vector<std::unordered_map<int, Action>>;
    // Goto[state, nonTerminal] = ...state
    using GotoTable = std::vector<std::unordered_map<int, int>>;
private:
    Grammar grammar;
    std::vector<std::unordered_set<LR1Item>> canonicalCollection;
    std::unordered_map<int, std::unordered_map<int, int>> transitions;
    ActionTable actionTable;
    GotoTable gotoTable;
public:
    explicit LR1TableBuilder(
            Grammar grammar,
            std::vector<std::unordered_set<LR1Item>> canonicalCollection,
            std::unordered_map<int, std::unordered_map<int, int>> transitions
    ) : grammar(std::move(grammar)),
        canonicalCollection(std::move(canonicalCollection)),
        transitions(std::move(transitions)) {
        computeActionAndGoto();
    }

    /// Ref: eac2 english. ver. p134
    void computeActionAndGoto() {
        actionTable = ActionTable(canonicalCollection.size(), std::unordered_map<int, Action>());
        gotoTable = GotoTable(canonicalCollection.size(), std::unordered_map<int, int>());
        for (int i = 0; i < canonicalCollection.size(); ++i) {
            // 这个状态是否有转移
            bool hasTransition = transitions.find(i) != transitions.cend();
            // Action表格
            for (const auto &item : canonicalCollection[i]) {
                // A -> b•cd, a
                if (item.isDotWithinIndices(grammar)) {
                    const auto &firstSymbol = item.firstSymbolAfterDot(grammar);
                    // 如果有 goto(i, firstSymbol) 的转移
                    if (hasTransition && transitions.at(i).find(firstSymbol) != transitions.at(i).cend()) {
                        // 判断是否是LR1
                        auto newAction = Actions::Shift(transitions.at(i).at(firstSymbol));
                        if (actionTable.at(i).find(firstSymbol) != actionTable.at(i).cend() &&
                            actionTable[i].at(firstSymbol) != Action(newAction)) {
                            throw std::runtime_error("Conflict actions\n");
                        }
                        actionTable[i].insert({firstSymbol, Actions::Shift(transitions.at(i).at(firstSymbol))});
                    }
                } else if (item == LR1Item(0, 1, grammar.terminatorIndex)) {
                    // S' -> S•, $的情况
                    auto newAction = Actions::Accept();
                    if (actionTable.at(i).find(grammar.terminatorIndex) != actionTable.at(i).cend() &&
                        actionTable[i].at(grammar.terminatorIndex) != Action(newAction)) {
                        throw std::runtime_error("Conflict actions\n");
                    }

                    actionTable[i].insert({grammar.terminatorIndex, newAction});
                } else {
                    // A -> b•, a的情况
                    auto newAction = Actions::Reduce(item.productionIndex);
                    if (actionTable.at(i).find(item.lookaheadIndex) != actionTable.at(i).cend() &&
                        actionTable[i].at(item.lookaheadIndex) != Action(newAction)) {
                        throw std::runtime_error("Conflict actions\n");
                    }

                    actionTable[i].insert({item.lookaheadIndex, newAction});
                }
            }
            // Goto表格
            for (int nonTerminalIndex = 0; nonTerminalIndex < grammar.nonTerminals.size(); ++nonTerminalIndex) {
                if (hasTransition && transitions.at(i).find(nonTerminalIndex) != transitions.at(i).cend()) {
                    gotoTable[i].insert({nonTerminalIndex, transitions.at(i).at(nonTerminalIndex)});
                }
            }
        }
    }

    const std::vector<std::unordered_set<LR1Item>> &getCanonicalCollection() const {
        return canonicalCollection;
    }

    const std::unordered_map<int, std::unordered_map<int, int>> &getTransitions() const {
        return transitions;
    }

    const ActionTable &getActionTable() const {
        return actionTable;
    }

    const GotoTable &getGotoTable() const {
        return gotoTable;
    }
};


#endif //NAIVECCOMPILER_LR1TABLEBUILDER_H
