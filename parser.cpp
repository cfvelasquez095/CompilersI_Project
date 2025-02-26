#include "parser.h"

void Parser::parse()
{
    tk = lexer.getNextToken();
    program();
    if ( tk != Token::Eof)
    {
        std::cerr << "\033[1;31mUnable to parse file.\033[0m" << std::endl;
    }
    
}

void Parser::program()
{
    subtypesSection();
    variableSection();
    subprogramDecl();
    if ( tk == Token::INICIO )
    {
        tk = lexer.getNextToken();
        statementCall();

        if ( tk == Token::FIN )
        {
            tk = lexer.getNextToken();
            std::cout << "\033[1;32mMiniLPP program successfully parsed.\033[0m" << std::endl;
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'FIN' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[0m" << std::endl;
        }
        
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'INICIO' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::subtypesSection()
{
    if ( tk == Token::TIPO )
    {
        subtypeDecl();

    } else
    {
        /* code */
    }
    
}

void Parser::subtypeDecl()
{
    if ( tk == Token::TIPO )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();

            if ( tk == Token::ES )
            {
                tk = lexer.getNextToken();
                type();

                if ( tk == Token::Eol )
                {
                    tk = lexer.getNextToken();
                    subtypeDecl();

                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }
                
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'ES' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            } 
            
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
        
    } else
    {
        /* epsilon */
    }
    
}

void Parser::variableSection()
{
    if ( tk == Token::ENTERO || tk == Token::BOOL || tk == Token::charConstant || tk == Token::ARREGLO )
    {
        variableDecl();

    } else
    {
        /* epsilon */
    }
    
}

void Parser::variableDecl()
{
    if ( tk == Token::ENTERO || tk == Token::BOOL || tk == Token::charConstant || tk == Token::ARREGLO )
    {
        type();
        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            multVar();

            if ( tk == Token::Eol )
            {
                tk = lexer.getNextToken();
                variableDecl();

            } else
            {
                std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.toString(tk) << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
            
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        /* epsilon */
    }
    
}

void Parser::multVar()
{
    if ( tk == Token::COMMA )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            multVar();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
        
    } else
    {
        /* epsilon */
    }
    
}

void Parser::type()
{
    if ( tk == Token::ENTERO )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::BOOL )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::charConstant )
    {
        tk = lexer.getNextToken();

    } else
    {
        arrayType();
    }
    
}

void Parser::arrayType()
{
    if ( tk == Token::ARREGLO )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::OpenBracket )
        {
            tk = lexer.getNextToken();

            if ( tk == Token::DECIMAL || tk == Token::BIN || tk == Token::HEX )
            {
                tk = lexer.getNextToken();

                if ( tk == Token::CloseBracket )
                {
                    tk = lexer.getNextToken();

                    if ( tk == Token::DE )
                    {
                        tk = lexer.getNextToken();
                        type();

                    } else
                    {
                        std::cerr << "\033[1;31mE: Expected 'DE' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                    }   
                } else
                {
                    std::cerr << "\033[1;31mE: Expected ']' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }   
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'intConstant' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }   
        } else
        {
            std::cerr << "\033[1;31mE: Expected '[' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }   
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'ARREGLO' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::subprogramDecl()
{
    if ( tk == Token::FUNCION || tk == Token::PROCEDIMIENTO )
    {
        subprogramHeader();
        if ( tk == Token::Eol )
        {
            tk = lexer.getNextToken();
            variableSection();

            if ( tk == Token::INICIO )
            {
                tk = lexer.getNextToken();
                statementCall();
                if ( tk == Token::FIN )
                {
                    tk = lexer.getNextToken();

                    if ( tk == Token::Eol )
                    {
                        tk = lexer.getNextToken();
                        subprogramDecl();

                    } else
                    {
                        std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                    }
                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'FIN' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'INICIO' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
        } else
        {
            /* epsilon */
        }
    } else
    {
        /* epsilon */
    }
    
    
}

void Parser::subprogramHeader()
{
    // functionHeader();
    // procedureHeader();
    if ( tk == Token::FUNCION )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            argumentCall();

            if ( tk == Token::COLON )
            {
                tk = lexer.getNextToken();
                type();

            } else
            {
                std::cerr << "\033[1;31mE: Expected ':' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else if ( tk == Token::PROCEDIMIENTO )
    {
        tk = lexer.getNextToken();
        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            argumentCall();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'FUNCION' or 'PROCEDIMIENTO' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
}

void Parser::functionHeader()
{
    if ( tk == Token::FUNCION )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            argumentCall();

            if ( tk == Token::COLON )
            {
                tk = lexer.getNextToken();
                type();

            } else
            {
                std::cerr << "\033[1;31mE: Expected ':' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'FUNCION' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::procedureHeader()
{
    if ( tk == Token::PROCEDIMIENTO )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            argumentCall();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'PROCEDIMIENTO' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::argumentDecl()
{
    if ( tk == Token::VAR )
    {
        tk = lexer.getNextToken();
        type();
        
        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            multArg();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        type();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            multArg();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    }
    
}

void Parser::multArg()
{
    if ( tk == Token::COMMA )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::VAR )
        {
            tk = lexer.getNextToken();
            type();

            if ( tk == Token::ID )
            {
                tk = lexer.getNextToken();
                multArg();

            } else
            {
                std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }    
        } else
        {
            type();

            if ( tk == Token::ID )
            {
                tk = lexer.getNextToken();
                multArg();

            } else
            {
                std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
        }
    } else
    {
        /* epsilon */
    }
    
}

void Parser::argumentCall()
{
    if ( tk == Token::OpenPar )
    {
        tk = lexer.getNextToken();
        if ( tk == Token::ClosePar )
        {
            tk = lexer.getNextToken();

        } else
        {
            argumentDecl();

            if ( tk == Token::ClosePar )
            {
                tk = lexer.getNextToken();
            } else
            {
                std::cerr << "\033[1;31mE: Expected ')' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }   
        }
    } else
    {
        std::cerr << "\033[1;31mE: Expected '(', 'var' or 'type' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::eolCall()
{
    if ( tk == Token::Eol )
    {
        tk = lexer.getNextToken();

    } else
    {
        /* code */
    }
    
}

void Parser::statement()
{
    if ( tk == Token::LLAMAR ) //statement LLAMAR
    {
        tk = lexer.getNextToken();

        if ( tk == Token::ID )
        {
            tk = lexer.getNextToken();
            exprCall();
            statementCall();
            
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }   
    } else if ( tk == Token::ESCRIBA ) //statement ESCRIBA
    {
        tk = lexer.getNextToken();
        argument();
        statementCall();

    } else if ( tk == Token::LEA ) //statement LEA
    {
        tk = lexer.getNextToken();
        lvalue();
        statementCall();

    } else if ( tk == Token::MIENTRAS ) //statement MIENTRAS
    {
        tk = lexer.getNextToken();
        expr();
        eolCall();
        if ( tk == Token::HAGA )
        {
            tk = lexer.getNextToken();

            if ( tk == Token::Eol )
            {
                tk = lexer.getNextToken();
                statementCall();

                if ( tk == Token::Eol )
                {
                    tk = lexer.getNextToken();

                    if ( tk == Token::FIN )
                    {
                        tk = lexer.getNextToken();

                        if ( tk == Token::MIENTRAS )
                        {
                            tk = lexer.getNextToken();
                            statementCall();

                        } else
                        {
                            std::cerr << "\033[1;31mE: Expected 'MIENTRAS' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                        }   
                    } else
                    {
                        std::cerr << "\033[1;31mE: Expected 'FIN' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                    }   
                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }   
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }   
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'HAGA' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }   
    } else if ( tk == Token::REPITA ) //statement REPITA
    {
        tk = lexer.getNextToken();
        tk = lexer.getNextToken();
        statementCall();

        if ( tk == Token::Eol )
        {
            tk = lexer.getNextToken();

            if ( tk == Token::HASTA )
            {
                tk = lexer.getNextToken();
                expr();
                statementCall();

            } else
            {
                std::cerr << "\033[1;31mE: Expected 'HASTA' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }   
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }

    } else if ( tk == Token::PARA ) //statement PARA
    {
        tk = lexer.getNextToken();
        lvalue();

        if ( tk == Token::ASSIGN )
        {
            tk = lexer.getNextToken();
            expr();

            if ( tk == Token::HASTA )
            {
                tk = lexer.getNextToken();
                expr();

                if ( tk == Token::HAGA )
                {
                    tk = lexer.getNextToken();
                    statementCall();

                    if ( tk == Token::Eol)
                    {
                        tk = lexer.getNextToken();
                        
                        if ( tk == Token::FIN )
                        {
                            tk = lexer.getNextToken();

                            if ( tk == Token::PARA )
                            {
                                tk = lexer.getNextToken();
                                statementCall();

                            } else
                            {
                                std::cerr << "\033[1;31mE: Expected 'PARA' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                            }   
                        } else
                        {
                            std::cerr << "\033[1;31mE: Expected 'FIN' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                        }   
                    } else
                    {
                        std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                    }
                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'HAGA' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }   
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'HASTA' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }   
        } else
        {
            std::cerr << "\033[1;31mE: Expected '<-' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }   
        
    } else if ( tk == Token::SI )
    {
        tk = lexer.getNextToken();
        expr();
        eolCall();

        if ( tk == Token::ENTONCES )
        {
            tk = lexer.getNextToken();
            statementCall();

                elseifStatement();
                elseBlock();
                if ( tk == Token::FIN )
                {
                    tk = lexer.getNextToken();
                    if ( tk == Token::SI )
                    {
                        tk = lexer.getNextToken();
                        statementCall();
                    } else
                    {
                        std::cerr << "\033[1;31mE: Expected 'SI' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                    }
                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'FIN' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }
           
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'ENTONCES' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else if ( tk == Token::ID )
    {
        lvalue();
        
        if ( tk == Token::ASSIGN )
        {
            tk = lexer.getNextToken();
            expr();
            statementCall();

        } else
        {
            std::cerr << "\033[1;31mE: Expected '<-' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }   
    } else if ( tk == Token::RETORNE)
    {
        tk = lexer.getNextToken();
        expr();
        statementCall();

    } else
    {
        std::cerr << "\033[1;31mE: Expected 'statement' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }  

}

void Parser::statementCall()
{
    if ( tk == Token::Eol )
    {
        tk = lexer.getNextToken();
        if ( tk == Token::ID || tk == Token::LLAMAR || tk == Token::ESCRIBA ||
             tk == Token::LEA || tk == Token::SI || tk == Token::MIENTRAS ||
             tk == Token::REPITA || tk == Token::PARA || tk == Token::RETORNE)
        {
            statement();
        }
    } else
    {
        /* epsilon */
    }

}

void Parser::elseifStatement()
{
    if ( tk == Token::SINO )
    {
        tk = lexer.getNextToken();

        if ( tk == Token::SI )
        {
            tk = lexer.getNextToken();
            expr();
            eolCall();
            
            if ( tk == Token::ENTONCES )
            {
                tk = lexer.getNextToken();
                statementCall();

                if ( tk == Token::Eol)
                {
                    tk = lexer.getNextToken();

                } else
                {
                    std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
                }
            } else
            {
                std::cerr << "\033[1;31mE: Expected 'ENTONCES' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
            }
        } else
        {
            std::cerr << "\033[1;31mE: Expected 'SI' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        /* epsilon */
    }

}

void Parser::elseBlock()
{
    if ( tk == Token::SINO )
    {
        tk = lexer.getNextToken();
        statementCall();

        if ( tk == Token::Eol )
        {
            tk = lexer.getNextToken();

        } else
        {
            std::cerr << "\033[1;31mE: Expected 'EOL' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
        }
    } else
    {
        /* epsilon */
    }
    
}

void Parser::argument()
{
    if ( tk == Token::stringConstant )
    {
        tk = lexer.getNextToken();
    } else
    {
        expr();
    }
    
}

void Parser::lvalue()
{
    if ( tk == Token::ID )
    {
        tk = lexer.getNextToken();
        lvalue_p();

    } else
    {
        std::cerr << "\033[1;31mE: Expected 'ID' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::lvalue_p()
{
    if ( tk == Token::OpenBracket )
    {
        expr();
        
        if ( tk == Token::CloseBracket )
        {
            tk = lexer.getNextToken();
        }

    } else if ( tk == Token::OpenPar )
    {
        exprCall();

    } else
    {
        /* epsilon */
    }
    
}

void Parser::expr()
{
    if ( tk == Token::ID )
    {
        lvalue();
        expr_p();

    } else if ( tk == Token::DECIMAL || tk == Token::HEX || tk == Token::BIN || tk == Token::charConstant || tk == Token::VERDADERO || tk == Token::FALSO )
    {
        constant();
        expr_p();

    } else if ( tk == Token::OpSub )
    {
        tk = lexer.getNextToken();
        expr();

    } else if ( tk == Token::NO )
    {
        tk = lexer.getNextToken();
        expr();

    } else if ( tk == Token::OpenPar )
    {
        tk = lexer.getNextToken();
        expr();

        if ( tk == Token::ClosePar )
        {
            tk = lexer.getNextToken();
        }
        
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'ID for Expresion' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;        
    }

}

void Parser::expr_p()
{
    if ( tk == Token::OpAdd || tk == Token::OpSub || tk == Token::OpMul || 
         tk == Token::DIV || tk == Token::MOD || tk == Token::OpLT || 
         tk == Token::OpGT || tk == Token::OpLTE || tk == Token::OpGTE ||
         tk == Token::OpEQ || tk == Token::NotEQ || tk == Token::Y || tk == Token::O )
    {
        binOP();
        expr();
    }
    
}

void Parser::multExpr()
{
    if ( tk == Token::COMMA )
    {
        tk = lexer.getNextToken();
        expr();

    } else
    {
        /* epsilon */
    }
    
}

void Parser::exprCall()
{
    if ( tk == Token::OpenPar )
    {
        tk = lexer.getNextToken();
        exprCall_p();

    } else
    {
        std::cerr << "\033[1;31mE: Expected '(' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;        
    }

}

void Parser::exprCall_p()
{
    if ( tk == Token::ID || tk == Token::DECIMAL || tk == Token::BIN ||
         tk == Token::HEX || tk == Token::charConstant || tk == Token::VERDADERO ||
         tk == Token::FALSO)
    {
        expr();
        multExpr();

        if ( tk == Token::ClosePar )
        {
            tk == lexer.getNextToken();
        }
        
    } else if ( tk == Token::ClosePar )
    {
        tk = lexer.getNextToken();
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'Expresion' or ')' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;        
    }
    
    
    
}

void Parser::binOP()
{
    if ( tk == Token::OpAdd )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpSub )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpMul )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::DIV )
    {
        tk = lexer.getNextToken();

    }else if ( tk == Token::MOD )
    {
        tk = lexer.getNextToken();
    } else if ( tk == Token::OpLT)
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpGT)
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpLTE)
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpGTE)
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::OpEQ )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::NotEQ)
    {
        tk = lexer.getNextToken();
    } else if ( tk == Token::Y )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::O )
    {
        tk = lexer.getNextToken();
    } else
    {
        std::cerr << "\033[1;31mE: Expected 'OPERAND' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
    
}

void Parser::constant()
{
    if ( tk == Token::DECIMAL || tk == Token::BIN || tk == Token::HEX )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::charConstant )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::VERDADERO )
    {
        tk = lexer.getNextToken();

    } else if ( tk == Token::FALSO )
    {
        tk = lexer.getNextToken();
        
    } else
    {
                std::cerr << "\033[1;31mE: Expected 'CONSTANT' but found: " << lexer.getText() << " at Line: " << lexer.line_num << "\033[m" << std::endl;
    }
        
}