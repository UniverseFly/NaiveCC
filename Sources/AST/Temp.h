//
// Created by 魏宇翔 on 2020/11/25.
//

#ifndef NAIVECCOMPILER_TEMP_H
#define NAIVECCOMPILER_TEMP_H


#include "Expression.h"
#include <sstream>
#include <vector>

/// 存放 3 地址码的中间变量t_i
class Temp : public Expression {
private:
    static unsigned numberCount;
private:
    /// 下标 i
    unsigned number;
public:
    Temp() : number(numberCount++) {}

    std::shared_ptr<const Expression> rValue() const override {
        return std::shared_ptr<const Expression>(static_cast<const Expression *>(this));
    }

    std::shared_ptr<const Expression> lValue() const override {
        return std::shared_ptr<const Expression>(static_cast<const Expression *>(this));
    }

    std::string getString() const override {
        std::ostringstream oss;
        oss << 't' << number;
        return oss.str();
    }

    std::string getValue() const override {
        return "Temp " + getString();
    }

    std::vector<std::shared_ptr<const GraphvizNode>>
    getChildren() const override{
        return {};
    }
};

unsigned Temp::numberCount = 0;


#endif //NAIVECCOMPILER_TEMP_H
