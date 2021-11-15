//
// Created by 魏宇翔 on 2020/11/23.
//

#ifndef NAIVECCOMPILER_VARIANTVISITOR_H
#define NAIVECCOMPILER_VARIANTVISITOR_H


// helper type for the visitor
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


#endif //NAIVECCOMPILER_VARIANTVISITOR_H
