//
// Created by 魏宇翔 on 2020/11/30.
//

#ifndef NAIVECCOMPILER_FILTERNULL_H
#define NAIVECCOMPILER_FILTERNULL_H


template<typename Collection>
Collection filterNull(const Collection &c) {
    Collection result;
    for (const auto &element : c) {
        if (element != nullptr) {
            result.push_back(element);
        }
    }
    return result;
}


#endif //NAIVECCOMPILER_FILTERNULL_H
