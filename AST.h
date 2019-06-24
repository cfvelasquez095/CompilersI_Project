#ifndef AST_H
#define AST_H

#include <memory>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cmath>

class ASTNode;
class BinaryExpr;
class Statement;
class FuncDeclStatement;
class GlobalContext;

#define express(name, oper, prec, op)                       \
class name##Expr: public BinaryExpr {                       \
public:                                                     \
    name##Expr(SPExpr expr1, SPExpr expr2):                 \
        BinaryExpr(std::move(expr1), std::move(expr2)) {};  \
                                                            \
    std::string toString() override {                       \
        return expr1->toString() + op + expr2->toString(); }\
    int getPrecedence() override { return prec; }           \
    int eval(GlobalContext &ctx) override {                 \
        return expr1->eval(ctx) oper expr2->eval(ctx); }    \
};

struct Value {
    std::string type;
    int value;
    int size = -1;
    int valsList[500];
};

struct Argument {
    std::string type, name;
};

struct Function {
    std::string type, id;
    SPStatementList stmts;
};

struct SinoSi{
    SPExpr exprEval;
    SPStatementList stmts;
};

using SPASTNode = std::shared_ptr<ASTNode>;
using SPASTNodeList = std::list<SPASTNode>;
using SPExpr = std::shared_ptr<BinaryExpr>;
using SPExprList = std::list<SPExpr>;
using SPStatement = std::shared_ptr<Statement>;
using SPStatementList = std::list<SPStatement>;

class GlobalContext {
public:
    GlobalContext() {};

    std::unordered_map<std::string, struct Value> vars;
    std::unordered_map<std::string, struct Function> funcs;
    int returnValue = -1;
    bool returnFunction = false;
};

class ASTNode
{
public:
    virtual std::string toString() = 0;
};

class Statement: public ASTNode
{
public:
    virtual void exec(GlobalContext &ctx) = 0;
};

class BinaryExpr : public ASTNode
{
public:
    BinaryExpr(SPExpr expr1, SPExpr expr2):
    expr1(std::move(expr1)), expr2(std::move(expr2)) {};

    virtual int getPrecedence() = 0;
    virtual int eval(GlobalContext& ctx) = 0;

    SPExpr expr1;
    SPExpr expr2;
};

express(Add, +, 2, "+");
express(Sub, -, 2, "-");
express(Or, ||, 2, "O");
express(And, &&, 3, "Y");
express(Mod, %, 3, "Mod");
express(Mul, *, 3, "*");
express(Div, /, 3, "Div");
express(Equal, ==, 1, "=");
express(NotEqual, !=, 1, "<>");
express(Greater, >, 1, ">");
express(GreaterEqual, >=, 1, ">=");
express(Less, <, 1, "<");
express(LessEqual, <=, 1, "<");

#endif