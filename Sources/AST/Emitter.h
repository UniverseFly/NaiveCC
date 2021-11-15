//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_EMITTER_H
#define NAIVECCOMPILER_EMITTER_H


#include <iostream>
#include <string>

struct Emitter {
    static void emit(const std::string &s) {
        std::cout << s << std::endl;
    }
};


#endif //NAIVECCOMPILER_EMITTER_H
