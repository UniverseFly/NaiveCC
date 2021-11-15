//
// Created by 魏宇翔 on 2020/11/21.
//

#ifndef NAIVECCOMPILER_TOKENKIND_H
#define NAIVECCOMPILER_TOKENKIND_H


/// 词法单元的种类
enum class TokenKind {
    Unknown, // 未知的
    Terminator, // 字符串末尾
    ID, Number, Semicolon, Comma, LeftBrace, RightBrace,
    LeftParenthesis, RightParenthesis, Comment,
    Int, Void, If, Else, While, Return, Assignment,
    RelationOperator, Plus, Minus, Multiply, Divide,
    Epsilon
};


#endif //NAIVECCOMPILER_TOKENKIND_H
