//
// Created by 魏宇翔 on 2020/11/22.
//

#ifndef NAIVECCOMPILER_ACTION_H
#define NAIVECCOMPILER_ACTION_H


#include <variant>

/// Action 表存放的东西（比如r5啊、s3、accept）
namespace Actions {
    struct Shift {
        const int state;

        explicit Shift(int state) : state(state) {}

        bool operator==(const Shift &rhs) const { return state == rhs.state; }

        bool operator!=(const Shift &rhs) const { return !(*this == rhs); }
    };

    struct Reduce {
        const int productionIndex;

        explicit Reduce(int productionIndex) : productionIndex(productionIndex) {}

        bool operator==(const Reduce &rhs) const { return productionIndex == rhs.productionIndex; }

        bool operator!=(const Reduce &rhs) const { return !(*this == rhs); }
    };

    struct Accept {
        bool operator==(const Accept &rhs) const { return true; }

        bool operator!=(const Accept &rhs) const { return !(*this == rhs); }
    };
};

using Action = std::variant<Actions::Shift, Actions::Reduce, Actions::Accept>;


#endif //NAIVECCOMPILER_ACTION_H
