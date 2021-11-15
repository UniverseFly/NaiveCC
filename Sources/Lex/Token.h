//
// Created by 魏宇翔 on 2020/11/21.
//

#ifndef NAIVECCOMPILER_TOKEN_H
#define NAIVECCOMPILER_TOKEN_H


#include <string>
#include "TokenKind.h"

/// 词法单元
class Token {
public:
    TokenKind kind;
    std::string lexeme;
public:
    Token(TokenKind kind, std::string lexeme) : kind(kind), lexeme(std::move(lexeme)) {}

    const TokenKind &getKind() const { return kind; }

    const std::string &getLexeme() const { return lexeme; }

    std::string stringRepresentation() const {
        return "<" + kindStringRepresentation(kind) + ", " + lexeme + ">";
    }

    /// 没什么用的函数，只是为了得到类型的string形式方便输出
    static std::string kindStringRepresentation(TokenKind kind) {
        switch (kind) {
            case TokenKind::Unknown:
                return "未知";
            case TokenKind::Terminator:
                return "EOF";
            case TokenKind::ID:
                return "标识符";
            case TokenKind::Number: {
                return "数字";
            }
            case TokenKind::Semicolon:
                return "分号";
            case TokenKind::Comma:
                return "逗号";
            case TokenKind::LeftBrace:
                return "左大括号";
            case TokenKind::RightBrace:
                return "右大括号";
            case TokenKind::LeftParenthesis:
                return "左圆括号";
            case TokenKind::RightParenthesis:
                return "右圆括号";
            case TokenKind::Comment:
                return "注释";
            case TokenKind::Int:
                return "int类型";
            case TokenKind::Void:
                return "void类型";
            case TokenKind::If:
                return "关键字if";
            case TokenKind::Else:
                return "关键字else";
            case TokenKind::While:
                return "关键字while";
            case TokenKind::Return:
                return "关键字return";
            case TokenKind::Assignment:
                return "赋值";
            case TokenKind::RelationOperator:
                return "比较运算符";
            case TokenKind::Plus:
                return "加法";
            case TokenKind::Minus:
                return "减法";
            case TokenKind::Multiply:
                return "乘法";
            case TokenKind::Divide:
                return "除法";
            case TokenKind::Epsilon:
                return "空";
        }
    }
};


#endif //NAIVECCOMPILER_TOKEN_H
