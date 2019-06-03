#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

class Parser
{
public:
    Parser(Lexer &lexer) : lexer(lexer) {};
    void parse();
private:
    Lexer &lexer;
    Token tk;

    void program();
    void subtypesSection();
    void subtypeDecl();
    void variableSection();
    void variableDecl();
    void multVar();
    void type();
    void arrayType();
    void subprogramDecl();
    void subprogramHeader();
    void functionHeader();
    void procedureHeader();
    void argumentDecl();
    void multArg();
    void argumentCall();
    void eolCall();
    void statement();
    void statementCall();
    void ifStatement();
    void elseifStatement();
    void elseBlock();
    void argument();
    void lvalue();
    void expr();
    void multExpr();
    void exprCall();
    void binOP();
    void arithOP();
    void relOP();
    void eqOP();
    void condOP();
    void constant();
    void boolConstant();

};
#endif