//
// Created by 魏宇翔 on 2020/11/23.
//

#ifndef NAIVECCOMPILER_GRAMMAR_H
#define NAIVECCOMPILER_GRAMMAR_H


#include "../Lex/TokenKind.h"
#include "../AST/ASTInclude.h"
#include <functional>
#include <deque>
#include <variant>

// todo: 这里必须要把初始化弄的安全！！！
// todo: Token 的 public 成员有非const的，需要修改一下接口
class Grammar {
public:
    /// 语法符号既可以是终结符、也可以是非终结符
    /// 产生式就是一个二元祖，左边是非终结符，右边都可以（都用的下标，利用正负确定是终结符还是非终结符）
    /// index >= 0 就从nonTerminals[index]，< 0 从terminals[terminals.size() + index] 取
    /// vector 是空代表 epsilon，虽然有epsilonIndex，但它不能在产生式里显示构造，terminator同理
    /// 产生式还可以有code（构建ast用的）
    struct Production {
        const int first;
        const std::vector<int> second;
        /// 根据得到的nodes计算Node
        const std::function<SemanticValue(const std::deque<SemanticValue> &nodes)> code;

        Production(int first, std::vector<int> second,
                   std::function<SemanticValue(const std::deque<SemanticValue> &)> code = [](auto) {
                       return std::shared_ptr<Expression>(
                               nullptr);
                   }) : first(first), second(std::move(second)), code(std::move(code)) {}
    };
    // using Production = std::pair<int, std::vector<int>>;
public:
    const std::vector<std::string> nonTerminals;
    const std::vector<TokenKind> terminals;
    const std::vector<Production> productions;

    /// 非终结符到产生式的映射
    const std::unordered_map<int, std::vector<int>> productionsOfNonTerminal;
    /// epsilon 的下标（作为非终结符，等于 -terminals.size() - 1）
    const int epsilonIndex;
    /// terminatorIndex == -terminals.size() - 2
    const int terminatorIndex;
    /// 终结符对应的vector下标
    const std::unordered_map<TokenKind, int> indexOfTerminal;
private:
    /// first 集合的存储结构
    std::unordered_map<int, std::unordered_set<int>> firstSet;
public:
    // TODO: 最好在初始化不要用到this
    Grammar(std::vector<std::string> nonTerminals,
            const std::vector<TokenKind> &terminals,
            std::vector<Production> productions) :
            nonTerminals(std::move(nonTerminals)),
            terminals(terminals),
            productions(std::move(productions)),
            productionsOfNonTerminal(getProductionsOfNonTerminal()),
            indexOfTerminal(getIndexOfTerminal()),
            epsilonIndex(-int(terminals.size()) - 1),
            terminatorIndex(-int(terminals.size()) - 2) {
        calculateFirstSet();
    }

    /// 接口函数，查firstSet表
    const std::unordered_set<int> &first(int symbol) const {
        return firstSet.at(symbol);
    }

    /// 一串symbol的first，不仅作为接口，还用在了构造过程
    std::unordered_set<int> first(const std::vector<int> &symbols) const {
        std::unordered_set<int> result;
        int index = 0;
        for (; index != symbols.size(); ++index) {
            // 当前遇到的符号的first
            const auto &firstOfSymbolIndex = firstSet.at(symbols[index]);
            // 先全部insert
            for (const auto &element : firstOfSymbolIndex) {
                result.insert({element});
            }
            // 如果找到了epsilon则减去epsilon观察下一个，没有找到epsilon，就不用继续了，
            if (firstOfSymbolIndex.find(epsilonIndex) != firstOfSymbolIndex.cend()) {
                result.erase(epsilonIndex);
            } else {
                break;
            }
        }
        // 如果循环走了全部，说明都包含epsilon，从而就插入
        if (index == symbols.size()) {
            result.insert({epsilonIndex});
        }
        return result;
    }

    std::string stringOfSymbol(int index) const {
        if (index > 0) {
            return nonTerminals[index];
        } else if (index == epsilonIndex) {
            return "ε";
        } else if (index == terminatorIndex) {
            return "#";
        }
        return Token::kindStringRepresentation(terminals[terminals.size() + index]);
    }

    std::string stringOfProduction(int index) const {
        const auto &nonTerminal = productions[index].first;
        const auto &terminals = productions[index].second;

        std::string result = stringOfSymbol(nonTerminal) + " -> ";
        for (const auto &terminal : terminals) {
            result += stringOfSymbol(terminal) + " ";
        }
        return result;
    }

private:
    /// 计算First集合，参考 Engineering a Compiler English Version P104
    void calculateFirstSet() {
        // 终结符的first
        for (int i = 0; i < terminals.size(); ++i) {
            int terminalIndex = i - int(terminals.size());
            firstSet.insert({terminalIndex, {terminalIndex}});
        }
        // 插入两个特殊符号，epsilon 和 terminator
        firstSet.insert({terminatorIndex, {terminatorIndex}});
        firstSet.insert({epsilonIndex, {epsilonIndex}});
        // 非终结符
        for (int i = 0; i < nonTerminals.size(); ++i) {
            firstSet.insert({i, {}});
        }
        bool isChanged = true;
        while (isChanged) {
            isChanged = false;
            for (const auto &production : productions) {
                const auto &rightPart = production.second;
                const auto &leftPart = production.first;
                // 用来计算是否要change
                auto tempSize = firstSet.at(leftPart).size();

                // 插入所有新的右部first
                for (const auto &newElement : first(rightPart)) {
                    firstSet.at(leftPart).insert({newElement});
                }

                if (firstSet.at(leftPart).size() != tempSize) {
                    isChanged = true;
                }
            }
        }
    }

    // Might cause bugs. Maybe this method should be made static.
    std::unordered_map<int, std::vector<int>>
    getProductionsOfNonTerminal() const {
        std::unordered_map<int, std::vector<int>> result;
        for (int i = 0; i < nonTerminals.size(); ++i) {
            result.insert({i, {}});
        }
        for (int i = 0; i < productions.size(); ++i) {
            result.at(productions[i].first).push_back(i);
        }
        return result;
    }

    std::unordered_map<TokenKind, int> getIndexOfTerminal() const {
        std::unordered_map<TokenKind, int> result;
        for (int i = 0; i < terminals.size(); ++i) {
            result.insert({terminals[i], i - int(terminals.size())});
        }
        result.insert({TokenKind::Terminator, terminatorIndex});
        result.insert({TokenKind::Epsilon, epsilonIndex});
        return result;
    }

    // int getEpsilonIndex() {
    //     for (int i = 0; i < terminals.size(); ++i) {
    //         if (terminals[i] == TokenKind::Epsilon) {
    //             return i;
    //         }
    //     }
    //     return 0;
    // }
};


#endif //NAIVECCOMPILER_GRAMMAR_H
