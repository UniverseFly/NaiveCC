cmake_minimum_required(VERSION 3.17)
project(NaiveCCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.15.0/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable(naivecc Sources/main.cpp Sources/Lex/Token.h Sources/Lex/TokenKind.h Sources/Lex/Lexer.h
        Sources/Parse/Parser.h
        Sources/Parse/LR1TableBuilder.h Sources/Parse/Action.h Sources/Parse/LR1Item.h
        Sources/Parse/DFABuilder.h Sources/Utilities/VariantVisitor.h
        Sources/Parse/Grammar.h Sources/AST/ArithOperator.h Sources/AST/Assignment.h Sources/AST/ASTInclude.h
        Sources/AST/Constant.h Sources/AST/Emitter.h Sources/AST/Expression.h
        Sources/AST/FunctionCall.h Sources/AST/Identifier.h Sources/AST/IfElseStatement.h Sources/AST/IfStatement.h
        Sources/AST/NullStatement.h Sources/AST/RelationOperator.h Sources/AST/ReturnStatement.h Sources/AST/Sequence.h
        Sources/AST/Statement.h Sources/AST/Temp.h Sources/AST/WhileStatement.h Sources/AST/SemanticValue.h Sources/Parse/StackState.h Sources/NaiveCGrammar.h Sources/Tree/Tree.h Sources/Tree/TreeGen.h Sources/Utilities/FilterNull.h Sources/AST/GraphvizNode.h)

target_link_libraries(naivecc Qt5::Widgets)
target_link_libraries(naivecc Qt5::Core)
