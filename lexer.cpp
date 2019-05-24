#include "lexer.h"

int Lexer::getNextChar() {
    int x = input.get();

    if(x == '\n'){
        line_num++;
    }
    
    return x;
}

void Lexer::ungetChar(int ch) {
    if(ch == '\n'){
        line_num--;
    }

    input.unget();
}

void Lexer::reportError(int ch) {
    std::cerr << "ERROR AT LINE: " << line_num << " - Unknown symbol: " << char(ch) << std::endl;
}

Token Lexer::getNextToken() {
    StateId state = StateId::Start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // Start
            case StateId::Start_q0:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::Start_q1;
                    text = "";
                }
                break;
            case StateId::Start_q1:
                if (ch == EOF) {
                    return Token::Eof;
                } else {
                    text += ch;
                    state = StateId::Start_q2;
                }
                break;
            case StateId::Start_q2:
                if (ch == ',') {
                    text += ch;
                    return Token::COMMA;
                } else if (ch == '_') {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else if (ch == '\'') {
                    state = StateId::charConst_q0;
                    ch = getNextChar();
                } else if (ch == '(') {
                    text += ch;
                    return Token::OpenPar;
                } else if (ch == '"') {
                    state = StateId::strConst_q0;
                    ch = getNextChar();
                } else if (ch == '[') {
                    text += ch;
                    return Token::OpenBracket;
                } else if (ch == ')') {
                    text += ch;
                    return Token::ClosePar;
                } else if (ch == ']') {
                    text += ch;
                    return Token::CloseBracket;
                } else if (ch == '/') {
                    state = StateId::Start_q7;
                    ch = getNextChar();
                } else if (ch == ':') {
                    text += ch;
                    return Token::COLON;
                } else {
                    text += ch;
                    state = StateId::resWords_q0;
                }
                break;
            case StateId::Start_q7:
                if (ch == '*') {
                    state = StateId::bcomm_q0;
                    ch = getNextChar();
                } else if (ch == '/') {
                    state = StateId::lcomm_q0;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'operators'
                    state = StateId::operators_q0;
                }
                break;
            // intConst
            case StateId::intConst_q0:
                if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = StateId::intConst_q4;
                    ch = getNextChar();
                } else if (ch == '0') {
                    text += ch;
                    state = StateId::intConst_q1;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'operators'
                    state = StateId::operators_q0;
                }
                break;
            case StateId::intConst_q1:
                if (ch == 'b') {
                    text += ch;
                    state = StateId::intConst_q3;
                    ch = getNextChar();
                } else if (ch == 'x') {
                    text += ch;
                    state = StateId::intConst_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::intConst_q4;
                }
                break;
            case StateId::intConst_q2:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f')) || ((ch >= 'A') && (ch <= 'F'))) {
                    text += ch;
                    state = StateId::intConst_q2;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::HEX;
                }
                break;
            case StateId::intConst_q3:
                if ((ch >= '0') && (ch <= '1')) {
                    text += ch;
                    state = StateId::intConst_q3;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::BIN;
                }
                break;
            case StateId::intConst_q4:
                if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = StateId::intConst_q4;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::DECIMAL;
                }
                break;
            // charConst
            case StateId::charConst_q0:
                if (ch == '\\') {
                    state = StateId::charConst_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::charConst_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::charConst_q1:
                if (ch == '\'') {
                    return Token::charConstant;
                } else {
                    // Trying next automaton 'operators'
                    state = StateId::operators_q0;
                }
                break;
            case StateId::charConst_q3:
                if (ch == '"') {
                    text += ch;
                    state = StateId::charConst_q1;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'operators'
                    state = StateId::operators_q0;
                }
                break;
            // strConst
            case StateId::strConst_q0:
                    text += ch;
                state = StateId::strConst_q1;
                ch = getNextChar();
                break;
            case StateId::strConst_q1:
                if (ch == '\\') {
                    state = StateId::strConst_q3;
                    ch = getNextChar();
                } else if (ch == '"') {
                    return Token::stringConstant;
                } else {
                    text += ch;
                    state = StateId::strConst_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::strConst_q3:
                if (ch == '"') {
                    text += ch;
                    state = StateId::strConst_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::strConst_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::strConst_q4:
                    text += ch;
                state = StateId::strConst_q1;
                ch = getNextChar();
                break;
            // lcomm
            case StateId::lcomm_q0:
                if ((ch == '\n') || (ch == EOF)) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::lcomm_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::lcomm_q1:
                if ((ch == EOF) || (ch == '\n')) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::lcomm_q1;
                    ch = getNextChar();
                }
                break;
            // bcomm
            case StateId::bcomm_q0:
                if (ch == EOF) {
                    return Token::Eof;
                } else {
                    state = StateId::bcomm_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::bcomm_q1:
                if (ch == '*') {
                    state = StateId::bcomm_q2;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    return Token::Eof;
                } else {
                    state = StateId::bcomm_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::bcomm_q2:
                if (ch == '/') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    return Token::Eof;
                } else {
                    state = StateId::bcomm_q1;
                    ch = getNextChar();
                }
                break;
            // operators
            case StateId::operators_q0:
                if (ch == '<') {
                    text += ch;
                    state = StateId::operators_q4;
                    ch = getNextChar();
                } else if (ch == '-') {
                    text += ch;
                    return Token::OpSub;
                } else if (ch == '*') {
                    text += ch;
                    return Token::OpMul;
                } else if (ch == '+') {
                    text += ch;
                    return Token::OpAdd;
                } else if (ch == '=') {
                    text += ch;
                    return Token::OpEQ;
                } else if (ch == '>') {
                    text += ch;
                    state = StateId::operators_q5;
                    ch = getNextChar();
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
            case StateId::operators_q4:
                if (ch == '-') {
                    text += ch;
                    return Token::ASSIGN;
                } else if (ch == '>') {
                    text += ch;
                    return Token::NotEQ;
                } else if (ch == '=') {
                    text += ch;
                    return Token::OpLTE;
                } else {
                    ungetChar(ch);
                    return Token::OpLT;
                }
                break;
            case StateId::operators_q5:
                if (ch == '=') {
                    text += ch;
                    return Token::OpGTE;
                } else {
                    ungetChar(ch);
                    return Token::OpGT;
                }
                break;
            // resWords
            case StateId::resWords_q0:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::resWords_q12;
                    ch = getNextChar();
                } else if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::iWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'j') || (ch == 'k') || (ch == 'z') || (ch == 'q') || (ch == 'w') || (ch == 'J') || (ch == 'K') || (ch == 'Z') || (ch == 'Q') || (ch == 'W')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = StateId::bWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::nWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::mWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::cWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::dWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::tWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::eWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::aWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::lWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'Y') || (ch == 'y')) {
                    text += ch;
                    state = StateId::resWords_q18;
                    ch = getNextChar();
                } else if ((ch == 'v') || (ch == 'V')) {
                    text += ch;
                    state = StateId::vWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::sWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'h') || (ch == 'H')) {
                    text += ch;
                    state = StateId::hWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'f') || (ch == 'F')) {
                    text += ch;
                    state = StateId::fWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::rWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::pWords_q0;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::intConst_q0;
                }
                break;
            case StateId::resWords_q12:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::O;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::resWords_q18:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::Y;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // aWords
            case StateId::aWords_q0:
                if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = StateId::aWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::aWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q1:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::aWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q10:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::ARCHIVO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q11:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::aWords_q12;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q12:
                if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::aWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q13:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::aWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q14:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::aWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q15:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ARREGLO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q2:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::aWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q3:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::aWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q4:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::ABRIR;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q5:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::aWords_q11;
                    ch = getNextChar();
                } else if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::aWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q6:
                if ((ch == 'h') || (ch == 'H')) {
                    text += ch;
                    state = StateId::aWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q7:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::aWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q8:
                if ((ch == 'V') || (ch == 'v')) {
                    text += ch;
                    state = StateId::aWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q9:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::aWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // bWords
            case StateId::bWords_q0:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::bWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q1:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::bWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q2:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::bWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q3:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::bWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q4:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::bWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q5:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::bWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q6:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::bWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q7:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::BOOL;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // cWords
            case StateId::cWords_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::cWords_q2;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::cWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::cWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q1:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::cWords_q6;
                    ch = getNextChar();
                } else if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::cWords_q4;
                    ch = getNextChar();
                } else if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::cWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q10:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::cWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q11:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::cWords_q12;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q12:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::cWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q13:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q14:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::CARACTER;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q15:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::CASO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q16:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q17;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q17:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::cWords_q18;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q18:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::cWords_q19;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q19:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::CERRAR;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q2:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q16;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q20:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::cWords_q21;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q21:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::COMO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q3:
                if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::cWords_q20;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q4:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::cWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::cWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q6:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::cWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q7:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::cWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q8:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::cWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q9:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::CADENA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // dWords
            case StateId::dWords_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::dWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::dWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::dWords_q1:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::DE;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::dWords_q3:
                if ((ch == 'v') || (ch == 'V')) {
                    text += ch;
                    state = StateId::dWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::dWords_q4:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::DIV;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // eWords
            case StateId::eWords_q0:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::eWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::eWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q1:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::eWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q10:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::eWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q11:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ENTONCES;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q13:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ES;
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::eWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q14:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q15:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::eWords_q16;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q16:
                if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = StateId::eWords_q17;
                    ch = getNextChar();
                } else if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::eWords_q21;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q17:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::eWords_q18;
                    ch = getNextChar();
                } else if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::eWords_q19;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q18:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ESCRIBA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q19:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q20;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q2:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::eWords_q3;
                    ch = getNextChar();
                } else if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::eWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q20:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ESCRIBIR;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q21:
                if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::eWords_q22;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q22:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q23;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q23:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::eWords_q24;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q24:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::ESCRITURA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q3:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q4:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::eWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q5:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::ENTERO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q7:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::eWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q8:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::eWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q9:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::eWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // fWords
            case StateId::fWords_q0:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::fWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::fWords_q6;
                    ch = getNextChar();
                } else if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::fWords_q12;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q1:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::fWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q10:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::FINAL;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q12:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::fWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q13:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::fWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q14:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::fWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q15:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::fWords_q16;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q16:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::fWords_q17;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q17:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::FUNCION;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q2:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::fWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q3:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::fWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q4:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::FALSO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q6:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::fWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q7:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::fWords_q9;
                    ch = getNextChar();
                } else if ((ch == ' ') || (ch == '\n')) {
                    return Token::FIN;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q9:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::fWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // hWords
            case StateId::hWords_q0:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::hWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q1:
                if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::hWords_q2;
                    ch = getNextChar();
                } else if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::hWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q2:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::hWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q3:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::HAGA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q5:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::hWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q6:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::hWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q7:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::HASTA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // iWords
            case StateId::iWords_q0:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::iWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q1:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::iWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q2:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::iWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q3:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::iWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q4:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::iWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q5:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::INICIO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // lWords
            case StateId::lWords_q0:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::lWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::lWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q1:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::lWords_q2;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::lWords_q4;
                    ch = getNextChar();
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::lWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q11:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::LEER;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q13:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::lWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q14:
                if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::lWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q15:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::lWords_q16;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q16:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::lWords_q17;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q17:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::LLAMAR;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q2:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::LEA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q3:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::lWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q4:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::lWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q5:
                if ((ch == 'U') || (ch == 'u')) {
                    text += ch;
                    state = StateId::lWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q6:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::lWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q7:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::lWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q8:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::LECTURA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // mWords
            case StateId::mWords_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::mWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::mWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q1:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::mWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q10:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::MOD;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q2:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::mWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q3:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::mWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q4:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::mWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::mWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q6:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::mWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q7:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::MIENTRAS;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q9:
                if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::mWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // nWords
            case StateId::nWords_q0:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::nWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::nWords_q1:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::nWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::nWords_q2:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::NO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // pWords
            case StateId::pWords_q0:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::pWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::pWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q1:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::pWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q10:
                if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::pWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q11:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::pWords_q12;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q12:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::pWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q13:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::pWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q14:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::pWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q15:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::pWords_q16;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q16:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::PROCEDIMIENTO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q2:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::pWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q3:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::PARA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q5:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::pWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q6:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::pWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q7:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::pWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q8:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::pWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q9:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::pWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // rWords
            case StateId::rWords_q0:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::rWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q1:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::rWords_q17;
                    ch = getNextChar();
                } else if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::rWords_q12;
                    ch = getNextChar();
                } else if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::rWords_q5;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::rWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q10:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::REGISTRO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q12:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::rWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q13:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::rWords_q14;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q14:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::rWords_q15;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q15:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::REPITA;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q17:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::rWords_q18;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q18:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::rWords_q19;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q19:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::rWords_q20;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q2:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::rWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q20:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::rWords_q21;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q21:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::RETORNE;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q3:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::REAL;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q5:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::rWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q6:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::rWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q7:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::rWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q8:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::rWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q9:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::rWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // sWords
            case StateId::sWords_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::sWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::sWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q1:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::sWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q11:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::SI;
                } else if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::sWords_q12;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q12:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::sWords_q13;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q13:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::SINO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q2:
                if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::sWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q3:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::sWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q4:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::sWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q5:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::sWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q6:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::sWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q7:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::sWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q8:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::sWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q9:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::SECUENCIAL;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // tWords
            case StateId::tWords_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::tWords_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q1:
                if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::tWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q2:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::tWords_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q3:
                if ((ch == ' ') || (ch == '\n')) {
                    return Token::TIPO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // vWords
            case StateId::vWords_q0:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::vWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::vWords_q4;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q1:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::vWords_q2;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q10:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::vWords_q11;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q11:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::VERDADERO;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q2:
                if ((ch == '\n') || (ch == ' ')) {
                    return Token::VAR;
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q4:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::vWords_q5;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q5:
                if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::vWords_q6;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q6:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::vWords_q7;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q7:
                if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::vWords_q8;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q8:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::vWords_q9;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q9:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::vWords_q10;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::id_q0;
                }
                break;
            // id
            case StateId::id_q0:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::id_q1;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ID;
                }
                break;
            case StateId::id_q1:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q1;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ID;
                }
                break;
        }
    }
}
const char *Lexer::toString(Token tk) {
    switch (tk) {
        case Token::VERDADERO: return "VERDADERO";
        case Token::REPITA: return "REPITA";
        case Token::REGISTRO: return "REGISTRO";
        case Token::PROCEDIMIENTO: return "PROCEDIMIENTO";
        case Token::MOD: return "MOD";
        case Token::LEA: return "LEA";
        case Token::SECUENCIAL: return "SECUENCIAL";
        case Token::LLAMAR: return "LLAMAR";
        case Token::LEER: return "LEER";
        case Token::INICIO: return "INICIO";
        case Token::HASTA: return "HASTA";
        case Token::HAGA: return "HAGA";
        case Token::FIN: return "FIN";
        case Token::FALSO: return "FALSO";
        case Token::SINO: return "SINO";
        case Token::FUNCION: return "FUNCION";
        case Token::FINAL: return "FINAL";
        case Token::VAR: return "VAR";
        case Token::LECTURA: return "LECTURA";
        case Token::ESCRIBIR: return "ESCRIBIR";
        case Token::ESCRIBA: return "ESCRIBA";
        case Token::ES: return "ES";
        case Token::DECIMAL: return "DECIMAL";
        case Token::ESCRITURA: return "ESCRITURA";
        case Token::OpEQ: return "OpEQ";
        case Token::CloseBracket: return "CloseBracket";
        case Token::OpMul: return "OpMul";
        case Token::CADENA: return "CADENA";
        case Token::ID: return "ID";
        case Token::COLON: return "COLON";
        case Token::COMMA: return "COMMA";
        case Token::REAL: return "REAL";
        case Token::ARCHIVO: return "ARCHIVO";
        case Token::Eof: return "Eof";
        case Token::ENTONCES: return "ENTONCES";
        case Token::BOOL: return "BOOL";
        case Token::MIENTRAS: return "MIENTRAS";
        case Token::DE: return "DE";
        case Token::SI: return "SI";
        case Token::BIN: return "BIN";
        case Token::charConstant: return "charConstant";
        case Token::OpSub: return "OpSub";
        case Token::DIV: return "DIV";
        case Token::NO: return "NO";
        case Token::CARACTER: return "CARACTER";
        case Token::OpenPar: return "OpenPar";
        case Token::OpenBracket: return "OpenBracket";
        case Token::ClosePar: return "ClosePar";
        case Token::Y: return "Y";
        case Token::stringConstant: return "stringConstant";
        case Token::NotEQ: return "NotEQ";
        case Token::OpLTE: return "OpLTE";
        case Token::ARREGLO: return "ARREGLO";
        case Token::RETORNE: return "RETORNE";
        case Token::ASSIGN: return "ASSIGN";
        case Token::OpLT: return "OpLT";
        case Token::ENTERO: return "ENTERO";
        case Token::HEX: return "HEX";
        case Token::OpGTE: return "OpGTE";
        case Token::PARA: return "PARA";
        case Token::OpGT: return "OpGT";
        case Token::ABRIR: return "ABRIR";
        case Token::TIPO: return "TIPO";
        case Token::CASO: return "CASO";
        case Token::COMO: return "COMO";
        case Token::OpAdd: return "OpAdd";
        case Token::O: return "O";
        case Token::CERRAR: return "CERRAR";
        default: return "Unknown";
    }
}