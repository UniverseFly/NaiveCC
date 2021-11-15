#include <iostream>
#include <fstream>
#include <iterator>
#include "Lex/Lexer.h"
#include "Parse/DFABuilder.h"
#include "Parse/Grammar.h"
#include "Parse/LR1TableBuilder.h"
#include "Parse/Parser.h"
#include "NaiveCGrammar.h"
#include "Tree/TreeGen.h"
#include <variant>
#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QMainWindow>

void printHelp() {
    std::cout << "这里是 Naive C Compiler，欢迎👏\n\n"
              << "使用方法: naivecc [option] file\n\n"
              << "[option] 可以是：\n"
              << "-t, --token\t\t只显示词法分析结果\n"
              << "-s, --step\t\t显示语法分析过程\n"
              << "-a, --ast\t\t生成中间代码\n"
              << "-g, --gen\t\t生成语法分析树 & AST的dot和pdf文件\n"
              << "--help    \t\t显示帮助"
              << std::endl;
}

void printNoInputFile() {
    std::cerr << "错误! 没有输入文件!!\n";
    std::cout << "需要帮助的话使用 --help 选项." << std::endl;
}

void printError() {
    std::cerr << "错误的使用方法!" << std::endl;
}

int token(const std::string &content) {
    Lexer(std::make_shared<const std::string>(content)).program();
    return 0;
}

int step(const std::string &content, int argc, char **argv) {
    Lexer lexer(std::make_shared<const std::string>(content));
    DFABuilder generator(NaiveCGrammar::naiveCGrammar);
    LR1TableBuilder tableGenerator(NaiveCGrammar::naiveCGrammar, generator.getCanonicalCollection(),
                                   generator.getTransitions());
    Parser parser(lexer, NaiveCGrammar::naiveCGrammar, tableGenerator.getActionTable(),
                  tableGenerator.getGotoTable());
    const auto result = parser.parse();

    QApplication app(argc, argv);

    auto widget = new QTableWidget(result.steps.size(), 4);

    widget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    widget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    widget->resize(1500, 1000);

    auto headerFont = widget->horizontalHeader()->font();
    headerFont.setPointSize(30);
    widget->horizontalHeader()->setFont(headerFont);

    widget->setHorizontalHeaderItem(0, new QTableWidgetItem("States"));
    widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Symbols"));
    widget->setHorizontalHeaderItem(2, new QTableWidgetItem("Action"));
    widget->setHorizontalHeaderItem(3, new QTableWidgetItem("Remaining Input"));
    widget->setTextElideMode(Qt::ElideNone);
    for (int i = 0; i < result.steps.size(); ++i) {
        const auto &step = result.steps[i];
        const auto states = new QTableWidgetItem(QString::fromStdString(step.states));
        const auto symbols = new QTableWidgetItem(QString::fromStdString(step.symbols));
        const auto action = new QTableWidgetItem(QString::fromStdString(step.action));
        const auto input = new QTableWidgetItem(QString::fromStdString(step.input));

        states->setFlags(states->flags() ^ Qt::ItemIsEditable);
        symbols->setFlags(symbols->flags() ^ Qt::ItemIsEditable);
        action->setFlags(action->flags() ^ Qt::ItemIsEditable);
        input->setFlags(input->flags() ^ Qt::ItemIsEditable);

        const auto &font = QFont("Comic Sans MS", 25);
        states->setFont(font);
        symbols->setFont(font);
        action->setFont(font);
        input->setFont(font);

        widget->setItem(i, 0, states);
        widget->setItem(i, 1, symbols);
        widget->setItem(i, 2, action);
        widget->setItem(i, 3, input);
    }
    // widget->resizeRowsToContents();
    // widget->resizeColumnsToContents();
    widget->show();

    std::cout << "请关注应用程序窗口" << std::endl;
    return QApplication::exec();
}

int ast(const std::string &content) {
    Lexer lexer(std::make_shared<const std::string>(content));
    DFABuilder generator(NaiveCGrammar::naiveCGrammar);
    LR1TableBuilder tableGenerator(NaiveCGrammar::naiveCGrammar, generator.getCanonicalCollection(),
                                   generator.getTransitions());
    Parser parser(lexer, NaiveCGrammar::naiveCGrammar, tableGenerator.getActionTable(),
                  tableGenerator.getGotoTable());
    const auto result = parser.parse();
    std::get<std::shared_ptr<const Statement>>(result.astRoot)->gen();
    return 0;
}

int gen(const std::string &content) {
    Lexer lexer(std::make_shared<const std::string>(content));
    DFABuilder generator(NaiveCGrammar::naiveCGrammar);
    LR1TableBuilder tableGenerator(NaiveCGrammar::naiveCGrammar, generator.getCanonicalCollection(),
                                   generator.getTransitions());
    Parser parser(lexer, NaiveCGrammar::naiveCGrammar, tableGenerator.getActionTable(),
                  tableGenerator.getGotoTable());
    const auto result = parser.parse();
    std::ofstream dotFile("../parseTree.dot");
    generateTree(dotFile, result.parseTree,
                 [](const Tree<StackState::NodeValue> &tree) {
                     return tree.getRoot();
                 },
                 [](const std::shared_ptr<TreeNode<StackState::NodeValue>> &node) {
                     return node->getChildren();
                 },
                 [](const std::shared_ptr<TreeNode<StackState::NodeValue>> &node) -> std::string {
                     return std::visit(overloaded{
                             [](int index) -> std::string {
                                 std::string result = "Index: " + std::to_string(index) + "\n";
                                 if (index > 0) {
                                     result += NaiveCGrammar::naiveCGrammar.nonTerminals[index];
                                 } else if (index == NaiveCGrammar::naiveCGrammar.epsilonIndex) {
                                     result += "ε";
                                 } else if (index == NaiveCGrammar::naiveCGrammar.terminatorIndex) {
                                     result += "#";
                                 }
                                 return result;
                             },
                             [](const Token &token) -> std::string {
                                 const std::string index = std::to_string(
                                         NaiveCGrammar::naiveCGrammar.indexOfTerminal.at(token.kind));
                                 std::string result =
                                         "Index: " + index + "\\n" + token.stringRepresentation();
                                 return result;
                             }}, node->getValue());
                 }

    );
    dotFile.close();
    system("dot -Tpdf ../parseTree.dot > ../parseTree.pdf");

    std::ofstream astDotFile("../ast.dot");
    generateTree(astDotFile, result.astRoot,
                 [](const SemanticValue &root) {
                     return std::get<std::shared_ptr<const Statement>>(root);
                 },
                 [](const std::shared_ptr<const GraphvizNode> &node) {
                     return node->getChildren();
                 },
                 [](const std::shared_ptr<const GraphvizNode> &node) -> std::string {
                     return node->getValue();
                 }
    );
    system("dot -Tpdf ../ast.dot > ../ast.pdf");
    system("open ../parseTree.pdf");
    system("open ../ast.pdf");
    std::cout << "生成了四个文件：../ast.dot, ../ast.pdf, ../parseTree.dot, ../parseTree.pdf" << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {
    switch (argc) {
        case 1:
            printNoInputFile();
            return -1;
        case 2: {
            if (std::string(argv[1]) == "--help") {
                printHelp();
                return 0;
            }
            std::ifstream file(argv[1]);
            if (!file) {
                std::cout << "无法打开文件  " << argv[1] << std::endl;
                return -1;
            }
            std::istreambuf_iterator<char> begin(file), end;
            const std::string content(begin, end);
            std::cout << "\n\n=========================----纯词法流输出----=========================" << std::endl;
            token(content);
            std::cout << "\n\n=========================----中间代码生成----=========================" << std::endl;
            ast(content);
            std::cout << "\n\n=========================----语法分析树和AST ()----=========================" << std::endl;
            gen(content);
            std::cout << "\n\n==========================----语法分析步骤表格----==========================" << std::endl;
            step(content, argc, argv);
            return 0;
        }
        case 3: {
            std::string option(argv[1]);
            if (option == "-t" || option == "--token") {
                std::ifstream file(argv[2]);
                if (!file) {
                    std::cout << "无法打开文件 " << argv[2] << std::endl;
                    return -1;
                }
                std::istreambuf_iterator<char> begin(file), end;
                const std::string content(begin, end);
                return token(content);
            } else if (option == "-a" || option == "--ast") {
                std::ifstream file(argv[2]);
                if (!file) {
                    std::cout << "无法打开文件 " << argv[2] << std::endl;
                    return -1;
                }
                std::istreambuf_iterator<char> begin(file), end;
                const std::string content(begin, end);
                return ast(content);
            } else if (option == "-g" || option == "--gen") {
                std::ifstream file(argv[2]);
                if (!file) {
                    std::cout << "无法打开文件 " << argv[2] << std::endl;
                    return -1;
                }
                std::istreambuf_iterator<char> begin(file), end;
                const std::string content(begin, end);
                return gen(content);
            } else if (option == "-s" || option == "--step") {
                std::ifstream file(argv[2]);
                if (!file) {
                    std::cout << "无法打开文件 " << argv[2] << std::endl;
                    return -1;
                }
                std::istreambuf_iterator<char> begin(file), end;
                const std::string content(begin, end);
                return step(content, argc, argv);
            }
            printError();
            return -1;
        }
        default:
            printError();
            return -1;
    }
}

// 语义分析可以参考LK的：每次reduce的时候利用if-else判断是哪个NT，对栈做相应操作
// 可以单独建立semantic stack，也可以直接 triple.
