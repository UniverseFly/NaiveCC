//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_LEXER_H
#define NAIVECCOMPILER_LEXER_H


#include <unordered_map>
#include <memory>
#include <cctype>
#include <iostream>
#include "Token.h"

class Lexer {
public:
    static constexpr char terminator = '#';
private:
    /// 词法分析器的当前行号
    int lineNumber = 1;
    /// 保留的关键字
    std::unordered_map<std::string, Token> reservedWords;
    /// Read-only buffer
    std::shared_ptr<const std::string> sourceBuffer;
    /// 正在读的index
    std::string::size_type peekIndex = 0;
public:
    explicit Lexer(decltype(sourceBuffer) sourceBuffer) : sourceBuffer(std::move(sourceBuffer)) {
        /// 保存关键字
        reserveKeywords();
    }

    void program() {
        while (true) {
            auto &&tokenString = scan().stringRepresentation();
            if (peek() == Lexer::terminator) { break; }
            std::cout << tokenString << std::endl;
        }
    }

    std::string remainingInput() const {
        return (*sourceBuffer).substr(peekIndex);
    }

    int getLineNumber() const {
        return lineNumber;
    }

    Token scan() {
        int state = 0;
        char prev;
        /// 代表标识符的词素
        std::string idLexeme;
        /// 代表数字的词素
        std::string numberLexeme;
        while (true) {
            switch (state) {
                case 0://空白符、制表符、换行符
                    if (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r') {
                        if (peek() == '\n') { lineNumber += 1; }
                        movePeekIndex();
                        state = 0;
                    } else if (isAlpha(peek())) {
                        idLexeme += peek();
                        movePeekIndex();
                        state = 1;
                    } else if (isDigit(peek())) {
                        numberLexeme = std::string(1, peek());
                        movePeekIndex();
                        state = 2;
                    } else if (peek() == '<' || peek() == '>' || peek() == '!' || peek() == '=') {
                        prev = peek();
                        movePeekIndex();
                        state = 3;
                    } else if (peek() == '+') {
                        movePeekIndex();
                        return {TokenKind::Plus, "+"};
                    } else if (peek() == '-') {
                        movePeekIndex();
                        return {TokenKind::Minus, "-"};
                    } else if (peek() == '*') {
                        movePeekIndex();
                        state = 4;
                    } else if (peek() == '/') {
                        movePeekIndex();
                        state = 5;
                    } else if (peek() == ',') {
                        movePeekIndex();
                        return {TokenKind::Comma, ","};
                    } else if (peek() == ';') {
                        movePeekIndex();
                        return {TokenKind::Semicolon, ";"};
                    } else if (peek() == '{') {
                        movePeekIndex();
                        return {TokenKind::LeftBrace, "{"};
                    } else if (peek() == '}') {
                        movePeekIndex();
                        return {TokenKind::RightBrace, "}"};
                    } else if (peek() == '(') {
                        movePeekIndex();
                        return {TokenKind::LeftParenthesis, "("};
                    } else if (peek() == ')') {
                        movePeekIndex();
                        return {TokenKind::RightParenthesis, ")"};
                    } else if (peek() == Lexer::terminator) {
                        return {TokenKind::Terminator, "#"};
                    } else {
                        // 返回的时当前的peek，之后再move
                        const auto &lastPeek = peek();
                        movePeekIndex();
                        return {TokenKind::Unknown, std::string(1, lastPeek)};
                    }
                    break;
                case 1://ID||reservedWords
                    if (isAlpha(peek()) || isDigit(peek())) {
                        state = 1;
                        idLexeme += peek();
                    } else if (reservedWords.find(idLexeme) != reservedWords.cend()) {
                        return reservedWords.at(idLexeme);
                    } else return {TokenKind::ID, idLexeme};
                    movePeekIndex();
                    break;
                case 2://num
                    if (isDigit(peek())) {
                        state = 2;
                        numberLexeme += peek();
                    } else {
                        return {TokenKind::Number, numberLexeme};
                    }
                    movePeekIndex();
                    break;
                case 3:
                    if (peek() == '=') {
                        movePeekIndex();
                        switch (prev) {
                            case '=':
                                return {TokenKind::RelationOperator, "=="};
                            case '<':
                                return {TokenKind::RelationOperator, "<="};
                            case '>':
                                return {TokenKind::RelationOperator, ">="};
                            case '!':
                                return {TokenKind::RelationOperator, "!="};
                            default:;
                        }
                    } else
                        switch (prev) {
                            case '=':
                                return {TokenKind::Assignment, "="};
                            case '<':
                                return {TokenKind::RelationOperator, "<"};
                            case '>':
                                return {TokenKind::RelationOperator, ">"};
                            case '!':
                                return {TokenKind::Unknown, std::string(1, '!') + peek()};
                            default:;
                        }
                    break;
                case 4:
                    if (peek() == '/') {
                        movePeekIndex();
                        return {TokenKind::Comment, "*/"};
                    } else {
                        return {TokenKind::Multiply, "*"};
                    }
                case 5:
                    if (peek() == '*') {
                        movePeekIndex();
                        return {TokenKind::Comment, "/*"};
                    } else if (peek() == '/') {
                        movePeekIndex();
                        return {TokenKind::Comment, "//"};
                    } else {
                        return {TokenKind::Divide, "/"};
                    }
                default:
                    break;
            }
        }
    }

private:
    /// Lexer::terminator 作为 EOF
    char peek() const {
        return peekIndex < sourceBuffer->size() ? (*sourceBuffer)[peekIndex] : Lexer::terminator;
    }

    /// 保存关键字
    void reserveKeywords() {
        /// 保留已有Token
        const auto reserveToken = [this](const Token &token) {
            this->reservedWords.insert({token.lexeme, token});
        };
        reserveToken({TokenKind::If, "if"});
        reserveToken({TokenKind::While, "while"});
        reserveToken({TokenKind::Void, "void"});
        reserveToken({TokenKind::Return, "return"});
        reserveToken({TokenKind::Int, "int"});
        reserveToken({TokenKind::Else, "else"});
    }

    void movePeekIndex() {
        peekIndex += 1;
    }

    /// 是数字时为真
    static bool isDigit(char c) { return ::isdigit(c); }

    /// 是字母时为真
    static bool isAlpha(char c) { return ::isalpha(c); }
};


#endif //NAIVECCOMPILER_LEXER_H
