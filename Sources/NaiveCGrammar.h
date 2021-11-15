//
// Created by 魏宇翔 on 2020/11/27.
//

#ifndef NAIVECCOMPILER_NAIVECGRAMMAR_H
#define NAIVECCOMPILER_NAIVECGRAMMAR_H


#include "Parse/Grammar.h"

// TODO: optimize the behavior of the semantic-value stack used during passing
namespace NaiveCGrammar {
    // 注意：nullptr 表示 AST 中不存在这个节点，而 NullStatement 表示空语句，二者概念不同。

    // program' -> program
    auto programPrime(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // program -> type id ( ) block
    auto program(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[4];
    }

    // type -> int
    auto type_int(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // type -> void
    auto type_void(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // block -> { declare sequence }
    auto block(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[2];
    }

    // declare -> epsilon
    auto declare_epsilon(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return std::shared_ptr<Statement>(nullptr);
    }

    // declare -> declare varDecl
    auto declare_varDecl(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return std::shared_ptr<Statement>(nullptr);
    }

    // varDecl -> int id;
    auto varDecl(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return std::shared_ptr<Statement>(nullptr);
    }

    // sequence -> epsilon
    auto sequence_epsilon(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return NullStatement::init();
    }

    // sequence -> sequence sentence
    auto sequence_sentence(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &sentence = std::get<std::shared_ptr<const Statement>>(nodes[0]);
        const auto &sequence = std::get<std::shared_ptr<const Statement>>(nodes[1]);
        return std::make_shared<const Sequence>(sentence, sequence);
    }

    // sentence -> ifStatement
    auto sentence_if(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // sentence -> whileStatement
    auto sentence_while(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // sentence -> returnStatement
    auto sentence_return(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // sentence -> assign
    auto sentence_assign(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // assign -> id = expr ;
    auto assign(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &id = std::make_shared<Identifier>(std::get<const Token>(nodes[0]).lexeme);
        // const auto &equal = std::get<const Token>(nodes[1]);
        const auto &expr = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const Assignment>(id, expr);
    }

    // returnStatement -> return returnValue
    auto returnStatement(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &returnValue = std::get<std::shared_ptr<const Expression>>(nodes[1]);
        return std::make_shared<ReturnStatement>(returnValue);
    }

    // returnValue -> epsilon
    auto returnValue_epsilon(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return std::shared_ptr<const Expression>(nullptr);
    }

    // returnValue -> expr
    auto returnValue_expr(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &expr = std::get<std::shared_ptr<const Expression>>(nodes[1]);
        return expr;
    }

    // whileStatement -> while ( expr ) block
    auto whileStatement(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &expr = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        const auto &block = std::get<std::shared_ptr<const Statement>>(nodes[4]);
        return std::make_shared<WhileStatement>(expr, block);
    }

    // ifStatement -> if ( expr ) block elseStatement
    auto ifStatement(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &expr = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        const auto &block = std::get<std::shared_ptr<const Statement>>(nodes[4]);
        const auto &elseStatement = std::get<std::shared_ptr<const Statement>>(nodes[5]);
        if (elseStatement == nullptr) {
            return std::make_shared<IfStatement>(expr, block);
        }
        return std::make_shared<IfElseStatement>(expr, block, elseStatement);
    }

    // elseStatement -> epsilon
    auto elseStatement_epsilon(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return std::shared_ptr<const Statement>(nullptr);
    }

    // elseStatement -> else block
    auto elseStatement_else(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &block = std::get<std::shared_ptr<const Statement>>(nodes[1]);
        return block;
    }

    // expr -> addExpr
    auto expr_addExpr(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // expr -> expr relop addExpr
    auto expr_relop(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &expr = std::get<std::shared_ptr<const Expression>>(nodes[0]);
        const auto &relop = RelationOperator::getKindFromToken(std::get<const Token>(nodes[1]));
        const auto &addExpr = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const RelationOperator>(expr, relop, addExpr);
    }

    // addExpr -> term
    auto addExpr_term(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // addExpr -> addExpr + term
    auto addExpr_plus(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &addExpr = std::get<std::shared_ptr<const Expression>>(nodes[0]);
        const auto &op = ArithOperator::getKindFromToken(std::get<const Token>(nodes[1]));
        const auto &term = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const ArithOperator>(addExpr, op, term);
    }

    // addExpr -> addExpr - term
    auto addExpr_minus(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &addExpr = std::get<std::shared_ptr<const Expression>>(nodes[0]);
        const auto &op = ArithOperator::getKindFromToken(std::get<const Token>(nodes[1]));
        const auto &term = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const ArithOperator>(addExpr, op, term);
    }

    // term -> factor
    auto term_factor(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // term -> term * factor
    auto term_mult(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &term = std::get<std::shared_ptr<const Expression>>(nodes[0]);
        const auto &op = ArithOperator::getKindFromToken(std::get<const Token>(nodes[1]));
        const auto &factor = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const ArithOperator>(term, op, factor);
    }

    // term -> term / factor
    auto term_div(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &term = std::get<std::shared_ptr<const Expression>>(nodes[0]);
        const auto &op = ArithOperator::getKindFromToken(std::get<const Token>(nodes[1]));
        const auto &factor = std::get<std::shared_ptr<const Expression>>(nodes[2]);
        return std::make_shared<const ArithOperator>(term, op, factor);
    }

    // factor -> num
    auto factor_num(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &num = Constant::getValueFromToken(std::get<const Token>(nodes[0]));
        return std::make_shared<Constant>(num);
    }

    // factor -> ( expr )
    auto factor_leftParenthesis(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[1];
    }

    // factor -> id
    auto factor_id(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        const auto &id = std::make_shared<Identifier>(std::get<const Token>(nodes[0]).getLexeme());
        return id;
    }

    // factor -> funcCall
    auto factor_funcCall(const std::deque<SemanticValue> &nodes) -> SemanticValue {
        return nodes[0];
    }

    // todo: 实现函数调用（不要用奇怪的接口）
    // funcCall -> id ( args )
    // auto funcCall(const std::deque<SemanticValue> &nodes) -> SemanticValue {
    //     const auto &id = std::make_shared<Identifier>(std::get<const Token>(nodes[0]).getLexeme());
    //     return id;
    // }

    //
    // auto args_argList(const std::deque<SemanticValue> &nodes) -> SemanticValue {
    //
    // }
    //
    // auto args_epsilon(const std::deque<SemanticValue> &nodes) -> SemanticValue {
    //
    // }
    //
    // auto argList(const std::deque<SemanticValue> &nodes) -> SemanticValue {
    //
    // }


    const std::vector<std::string> nonTerminals{
            "program'",
            "program",
            "type",
            "block",
            "declare",
            "varDecl",
            "sequence",
            "sentence",
            "assign",
            "return",
            "returnValue",
            "while",
            "if",
            "else",
            "expr",
            "addExpr",
            "term",
            "factor",
            "funcCall",
            "args",
            "argList"
    };
    const std::vector<TokenKind> terminals{
            TokenKind::ID,
            TokenKind::LeftParenthesis,
            TokenKind::RightParenthesis,
            TokenKind::LeftBrace,
            TokenKind::RightBrace,
            TokenKind::Semicolon,
            TokenKind::Assignment,
            TokenKind::Plus,
            TokenKind::Minus,
            TokenKind::Multiply,
            TokenKind::Divide,
            TokenKind::Comma,
            TokenKind::Int,
            TokenKind::Void,
            TokenKind::Number,
            TokenKind::While,
            TokenKind::If,
            TokenKind::Else,
            TokenKind::Return,
            TokenKind::RelationOperator
    };
    const std::vector<Grammar::Production> productions{
            Grammar::Production{0, {1}, programPrime},
            Grammar::Production{1, {2, -20, -19, -18, 3}, program},
            Grammar::Production{2, {-8}, type_int},
            Grammar::Production{2, {-7}, type_void},
            Grammar::Production{3, {-17, 4, 6, -16}, block},
            Grammar::Production{4, {}, declare_epsilon},
            Grammar::Production{4, {4, 5}, declare_varDecl},
            Grammar::Production{5, {-8, -20, -15}, varDecl},
            Grammar::Production{6, {}, sequence_epsilon},
            Grammar::Production{6, {6, 7}, sequence_sentence},
            Grammar::Production{7, {12}, sentence_if},
            Grammar::Production{7, {11}, sentence_while},
            Grammar::Production{7, {9}, sentence_return},
            Grammar::Production{7, {8}, sentence_assign},
            Grammar::Production{8, {-20, -14, 14, -15}, assign},
            Grammar::Production{9, {-2, 10, -15}, returnStatement},
            Grammar::Production{10, {}, returnValue_epsilon},
            Grammar::Production{10, {14}, returnValue_expr},
            Grammar::Production{11, {-5, -19, 14, -18, 3}, whileStatement},
            Grammar::Production{12, {-4, -19, 14, -18, 3, 13}, ifStatement},
            Grammar::Production{13, {}, elseStatement_epsilon},
            Grammar::Production{13, {-3, 3}, elseStatement_else},
            Grammar::Production{14, {15}, expr_addExpr},
            Grammar::Production{14, {14, -1, 15}, expr_relop},
            Grammar::Production{15, {16}, addExpr_term},
            Grammar::Production{15, {15, -13, 16}, addExpr_plus},
            Grammar::Production{15, {15, -12, 16}, addExpr_minus},
            Grammar::Production{16, {17}, term_factor},
            Grammar::Production{16, {16, -11, 17}, term_mult},
            Grammar::Production{16, {16, -10, 17}, term_div},
            Grammar::Production{17, {-6}, factor_num},
            Grammar::Production{17, {-19, 14, -18}, factor_leftParenthesis},
            Grammar::Production{17, {-20}, factor_id},
            Grammar::Production{17, {18}},
            Grammar::Production{18, {-20, -19, 19, -18}},
            Grammar::Production{19, {20}},
            Grammar::Production{19, {}},
            Grammar::Production{20, {14}},
            Grammar::Production{20, {20, -9, 14}}
    };
    const Grammar naiveCGrammar(nonTerminals, terminals, productions);

    // /// 将所给的产生式转换为下标表示
    // std::vector<Grammar::Production>
    // convertToIndexWiseProductions(std::vector<> production, const std::vector<TokenKind> &terminals) {
    //
    // }
}

#endif //NAIVECCOMPILER_NAIVECGRAMMAR_H
