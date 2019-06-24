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
    BinaryExpr();
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

class NoExpr : public BinaryExpr
{
public:
    NoExpr(SPExpr expr1) : expr1(std::move(expr1)) {};
    std::string toString() override{
        return "No " +expr1->toString();
    }
    int eval(GlobalContext& ctx) override{
        return !expr1->eval(ctx);
    }
    int getPrecedence() override{
        return 5;
    }

    SPExpr expr1;
};

class UnaryExpr : public BinaryExpr
{
public:
    UnaryExpr(SPExpr expr1) : expr1(std::move(expr1)) {};
    std::string toString() override{ return "- " + expr1->toString(); }
    int eval(GlobalContext& ctx) override{ return -expr1->eval(ctx); }
    int getPrecedence() override { return 5; }

    SPExpr expr1;
};

class IntConstant: public BinaryExpr
{
public:
    IntConstant(int value) : value(value) {};

    std::string toString() override {return std::to_string(value);}
    int eval(GlobalContext& ctx) override{ return value; }
    int getPrecedence() override { return 1; }
    int value;
};

class CharConstant: public BinaryExpr
{
public:
    CharConstant(char value) : value(value) {};
    std::string toString() override {
        char c = (char)value;
        std::string s(1,c);
        return s;
    }
    int eval(GlobalContext& ctx) override { return (int)value; }
    int getPrecedence() override{ return 1; }
    char value;
};

class BoolConstant: public BinaryExpr
{
public:
    BoolConstant(std::string value) : value(value) {};
    std::string toString() override {
        if(value == "VERDADERO" || value == "verdadero" ||
            value == "Verdadero")
        {
            return "Verdadero";
        }
        return "Falso";
    }

    int eval(GlobalContext& ctx) override {
        if(value == "VERDADERO" || value == "verdadero" ||
            value == "Verdadero")
        {
            return 1;
        }
        return 0;
    }

    int getPrecedence() override{ return 1; }

    std::string value;
};

class StringConstant: public BinaryExpr
{
public:
    StringConstant(std::string value): value(value) {};
    std::string toString() override {return value;}
    int eval(GlobalContext& ctx)override{ return -1; }
    int getPrecedence() override { return 5; }

    std::string value;
};

#endif