#include "parser.h"
#include <sstream>

int main(int argc, char const *argv[])
{
    /* code */
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <test_file>" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::fstream infile(argv[1]);

    Lexer lexer(infile);
    Parser parser(lexer);

    Token tkn;

    parser.parse();
    
    // while ((tkn = lexer.getNextToken()) != Token::Eof) {
    //     std::cout << "Token: " << lexer.toString(tkn);
    //     std::cout << " || Value: " << lexer.getText() << std::endl;
    // }

    return EXIT_SUCCESS;
}