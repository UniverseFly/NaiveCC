//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_PARSER_H
#define NAIVECCOMPILER_PARSER_H


#include "../Lex/Lexer.h"
#include "LR1TableBuilder.h"
#include "StackState.h"
#include "../Tree/Tree.h"
#include "../Utilities/VariantVisitor.h"
#include <stack>
#include <vector>


// TODO: construct a better interface for 'SemanticValue' of the analysis stack
class Parser {
public:
    /// 语法分析的结果
    struct ParsingResult {
        struct Step {
            std::string states;
            std::string symbols;
            std::string input;
            std::string action;
        };
        const SemanticValue astRoot;
        const Tree<StackState::NodeValue> parseTree;
        const std::vector<const Step> steps;
    };
private:
    Lexer lexer;
    const Grammar grammar;
    const LR1TableBuilder::ActionTable actionTable;
    const LR1TableBuilder::GotoTable gotoTable;
public:
    Parser(Lexer lexer,
           Grammar grammar,
           LR1TableBuilder::ActionTable actionTable,
           LR1TableBuilder::GotoTable gotoTable
    ) : lexer(std::move(lexer)), grammar(std::move(grammar)),
        actionTable(std::move(actionTable)), gotoTable(std::move(gotoTable)) {}

    ParsingResult parse() {
        /// 分析栈，两个元素一组压栈，左边的是文法符号，右边的是状态。
        std::vector<StackState> stack;
        stack.emplace_back(grammar.terminatorIndex, 0, std::shared_ptr<Expression>(nullptr),
                           std::make_shared<TreeNode<StackState::NodeValue>>(grammar.terminatorIndex));

        /// 用来记录steps的（不是核心）
        std::vector<const ParsingResult::Step> steps;
        /// 当前的step
        ParsingResult::Step currentStep;

        /// 上一次的input，让表格更加好理解
        /// Loop invariant:
        /// - At the beginning: lastInput = peek + remaining
        /// - At the end: currentStep.input == lastInput
        std::string lastInput = lexer.remainingInput();

        auto peek = lexer.scan();
        while (true) {
            // 计算steps log，和算法没什么关系
            const auto statesAndSymbols = getStatesAndSymbols(stack);
            currentStep.states = statesAndSymbols.first;
            currentStep.symbols = statesAndSymbols.second;

            const auto stackTop = stack.back();
            // 开始match不同的action
            bool loopBreak = false;
            // 判断词法错误
            if (peek.kind == TokenKind::Unknown) {
                reportError(peek);
            }
            auto peekIndex = grammar.indexOfTerminal.at(peek.kind);
            // 判断语法错误出错
            if (actionTable[stackTop.state].find(peekIndex) == actionTable[stackTop.state].cend()) {
                reportError(peek);
            }
            // match是哪个action
            std::visit(overloaded{
                    [&](const Actions::Reduce &reduce) {
                        // 计算"steps"，和算法无关
                        currentStep.action = "Reduce: " + grammar.stringOfProduction(reduce.productionIndex);
                        currentStep.input = lastInput;

                        /// 待规约的产生式 A -> b
                        const auto &reduceProduction = grammar.productions[reduce.productionIndex];
                        const auto &nonTerminal = reduceProduction.first;
                        const auto &terminals = reduceProduction.second;

                        /// 记录栈内terminals.size()个状态（SemanticValue）
                        std::deque<SemanticValue> rightPartNodes;
                        /// 记录parse tree的节点
                        std::deque<std::shared_ptr<TreeNode<StackState::NodeValue>>> parseTreeChildren;
                        for (const auto &_ : terminals) {
                            // 将所有栈里面的右部Node记录下来，共语法制导翻译调用
                            rightPartNodes.push_front(stack.back().astNode);
                            // 记录parse tree的children
                            parseTreeChildren.push_front(stack.back().parseTreeNode);
                            // 每次pop状态和符号
                            stack.pop_back();
                        }
                        /// 规约后的树根节点
                        const auto &reducedNode =
                                std::make_shared<TreeNode<StackState::NodeValue>>(nonTerminal, parseTreeChildren);
                        const auto gotoState = gotoTable[stack.back().state].at(nonTerminal);
                        // 这时候栈是xxs，push一个<A, goto(s, A), 新的ast node, 新的parse tree node>
                        stack.emplace_back(nonTerminal, gotoState, reduceProduction.code(rightPartNodes), reducedNode);
                    },
                    [&](const Actions::Shift &shift) {
                        // 计算"steps"，和算法无关
                        currentStep.action = "Shift to: " + std::to_string(shift.state) +
                                             ", " + peek.stringRepresentation();
                        currentStep.input = lastInput;
                        lastInput = lexer.remainingInput();

                        const auto parseTreeNode = std::make_shared<TreeNode<StackState::NodeValue>>(peek);
                        stack.emplace_back(peekIndex, shift.state, peek, parseTreeNode);
                        peek = lexer.scan();
                    },
                    [&](const Actions::Accept &accept) {
                        // 计算"steps"，和算法无关
                        currentStep.action = "Accept";
                        currentStep.input = lastInput;

                        loopBreak = true;
                    }
            }, actionTable[stackTop.state].at(peekIndex));

            steps.push_back(currentStep);

            if (loopBreak) {
                return {stack.back().astNode, Tree(stack.back().parseTreeNode), steps};
            }
        }
    }

private:
    /// 从stack中提取当前的状态、符号
    std::pair<std::string, std::string> getStatesAndSymbols(const std::vector<StackState> &stack) const {
        std::string states;
        std::string symbols;
        for (const auto &stackState : stack) {
            states += std::to_string(stackState.state) + " ";
            symbols += grammar.stringOfSymbol(stackState.symbol) + " ";
        }
        return {states, symbols};
    }

    void reportError(const Token &token) const {
        std::cerr << "第 " << lexer.getLineNumber() << " 行出现了错误的 token "
                  << token.stringRepresentation() << std::endl;

        std::exit(-1);
    }
    //
    // /// 从stack提取lexer remaining input 的表示，为了输出好看
    // static std::string getCodeRepr(std::string code) {
    //     code.erase(std::remove(code.begin(), code.end(), '\n'), code.end());
    //     code.erase(std::remove(code.begin(), code.end(), '\r'), code.end());
    //     return code;
    // }
};


#endif //NAIVECCOMPILER_PARSER_H
