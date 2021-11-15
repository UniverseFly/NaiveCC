# NaiveCC: a compiler frontend for a subset of C

This is a toy compiler frontend for a subset of the C programming language based on the LR(1) parsing technique that supports:
- Token stream output
- Intermediate code generation
- Parse tree & AST visualization
- LR(1) parsing step visualization

## Prerequisites

- QT 5
- C++ 17
- Graphviz

## Grammar Definition

![image-20211115145021842](README.assets/image-20211115145021842.png)

## Features

### Lexing

![lexing](README.assets/lexing.png)

### Intermediate code generation

![intermediateCode](README.assets/intermediateCode.png)

### Parse tree visualization

![image-20211115145117600](README.assets/image-20211115145117600.png)

### AST visualization

![image-20211115145151719](README.assets/image-20211115145151719.png)

### Parsing step table

![analysisTable](README.assets/analysisTable.png)

