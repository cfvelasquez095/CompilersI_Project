#include "lexer.h"
#include <sstream>

char data[300];

int main(int argc, char const *argv[])
{
    /* code */
    std::istringstream ins;
    std::fstream infile(argv[1]);

    Lexer lexer(infile);

    Token tkn;
    
    while ((tkn = lexer.getNextToken()) != Token::Eof) {
        std::cout << "Token: " << lexer.toString(tkn);
        std::cout << " || Value: " << lexer.getText() << std::endl;
    }

    return 0;
}