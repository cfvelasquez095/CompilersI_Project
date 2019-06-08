Token Lexer::getNextToken() {
    StateId state = StateId::Start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // Start
            case StateId::Start_q0:
                if ((ch == ' ') || (ch == '\t')) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q1;
                    text = "";
                }
                break;
            case StateId::Start_q1:
                if (ch == EOF) {
                    return Token::Eof;
                } else if (ch == '\n') {
                    state = StateId::Start_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q2;
                }
                break;
            case StateId::Start_q17:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = StateId::Start_q17;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::Eol;
                }
                break;
            case StateId::Start_q2:
                if (ch == '\'') {
                    state = StateId::charConst_q0;
                    ch = getNextChar();
                } else if (ch == '_') {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else if (ch == ')') {
                    text += ch;
                    return Token::ClosePar;
                } else if (ch == ',') {
                    text += ch;
                    return Token::COMMA;
                } else if (ch == ':') {
                    text += ch;
                    return Token::COLON;
                } else if (ch == '[') {
                    text += ch;
                    return Token::OpenBracket;
                } else if (ch == '"') {
                    state = StateId::strConst_q0;
                    ch = getNextChar();
                } else if (ch == '/') {
                    state = StateId::Start_q7;
                    ch = getNextChar();
                } else if (ch == ']') {
                    text += ch;
                    return Token::CloseBracket;
                } else if (ch == '(') {
                    text += ch;
                    return Token::OpenPar;
                } else {
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
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::lWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'H') || (ch == 'h')) {
                    text += ch;
                    state = StateId::hWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::aWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::iWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::dWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::cWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'z') || (ch == 'x') || (ch == 'q') || (ch == 'w') || (ch == 'u') || (ch == 'j') || (ch == 'k') || (ch == 'g') || (ch == 'Z') || (ch == 'X') || (ch == 'Q') || (ch == 'W') || (ch == 'U') || (ch == 'J') || (ch == 'K') || (ch == 'G')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::nWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'V') || (ch == 'v')) {
                    text += ch;
                    state = StateId::vWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'f') || (ch == 'F')) {
                    text += ch;
                    state = StateId::fWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::pWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::resWords_q12;
                    ch = getNextChar();
                } else if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::tWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = StateId::bWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::sWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'y') || (ch == 'Y')) {
                    text += ch;
                    state = StateId::resWords_q18;
                    ch = getNextChar();
                } else if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::mWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::rWords_q0;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::eWords_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::intConst_q0;
                }
                break;
            case StateId::resWords_q12:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::O;
                }
                break;
            case StateId::resWords_q18:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::Y;
                }
                break;
            // aWords
            case StateId::aWords_q0:
                if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = StateId::aWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::aWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q1:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::aWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q10:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ARCHIVO;
                }
                break;
            case StateId::aWords_q11:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::aWords_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q12:
                if ((ch == 'G') || (ch == 'g')) {
                    text += ch;
                    state = StateId::aWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q13:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::aWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q14:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::aWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q15:
                if ((ch == isalnum) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ARREGLO;
                }
                break;
            case StateId::aWords_q2:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::aWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q3:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::aWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q4:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ABRIR;
                }
                break;
            case StateId::aWords_q5:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::aWords_q6;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::aWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q6:
                if ((ch == 'H') || (ch == 'h')) {
                    text += ch;
                    state = StateId::aWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q7:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::aWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q8:
                if ((ch == 'V') || (ch == 'v')) {
                    text += ch;
                    state = StateId::aWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::aWords_q9:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::aWords_q10;
                    ch = getNextChar();
                } else {
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
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q1:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::bWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q2:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::bWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q3:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::bWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q4:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::bWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q5:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::bWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q6:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::bWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::bWords_q7:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::BOOL;
                }
                break;
            // cWords
            case StateId::cWords_q0:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::cWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::cWords_q3;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::cWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q1:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::cWords_q4;
                    ch = getNextChar();
                } else if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::cWords_q6;
                    ch = getNextChar();
                } else if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::cWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q10:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::cWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q11:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::cWords_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q12:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::cWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q13:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::cWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q14:
                if (isalnum(ch)) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::CARACTER;
                }
                break;
            case StateId::cWords_q15:
                if (isalnum(ch)) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::CASO;
                }
                break;
            case StateId::cWords_q16:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q17:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::cWords_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q18:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q19:
                if (isalnum(ch)) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::CERRAR;
                }
                break;
            case StateId::cWords_q2:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::cWords_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q20:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::cWords_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q21:
                if (isalnum(ch)) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::COMO;
                }
                break;
            case StateId::cWords_q3:
                if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::cWords_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q4:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::cWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::cWords_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q6:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::cWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q7:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::cWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q8:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::cWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::cWords_q9:
                if (isalnum(ch)) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::CADENA;
                }
                break;
            // dWords
            case StateId::dWords_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::dWords_q3;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::dWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::dWords_q1:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::DE;
                }
                break;
            case StateId::dWords_q3:
                if ((ch == 'v') || (ch == 'V')) {
                    text += ch;
                    state = StateId::dWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::dWords_q4:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::DIV;
                }
                break;
            // eWords
            case StateId::eWords_q0:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::eWords_q13;
                    ch = getNextChar();
                } else if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::eWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q1:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::eWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q10:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::eWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q11:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ENTONCES;
                }
                break;
            case StateId::eWords_q13:
                if (((ch >= 'a') && (ch <= 'b')) || ((ch >= 'd') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'B')) || ((ch >= 'D') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::eWords_q14;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ES;
                }
                break;
            case StateId::eWords_q14:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q15:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::eWords_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q16:
                if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = StateId::eWords_q17;
                    ch = getNextChar();
                } else if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::eWords_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q17:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::eWords_q18;
                    ch = getNextChar();
                } else if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::eWords_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q18:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ESCRIBA;
                }
                break;
            case StateId::eWords_q19:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q2:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::eWords_q7;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::eWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q20:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ESCRIBIR;
                }
                break;
            case StateId::eWords_q21:
                if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::eWords_q22;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q22:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::eWords_q23;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q23:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::eWords_q24;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q24:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ESCRITURA;
                }
                break;
            case StateId::eWords_q3:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::eWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q4:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::eWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q5:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::ENTERO;
                }
                break;
            case StateId::eWords_q7:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::eWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q8:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::eWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::eWords_q9:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::eWords_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            // fWords
            case StateId::fWords_q0:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::fWords_q6;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::fWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'U') || (ch == 'u')) {
                    text += ch;
                    state = StateId::fWords_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q1:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::fWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q10:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::FINAL;
                }
                break;
            case StateId::fWords_q12:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::fWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q13:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::fWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q14:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::fWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q15:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::fWords_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q16:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::fWords_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q17:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::FUNCION;
                }
                break;
            case StateId::fWords_q2:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::fWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q3:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::fWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q4:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::FALSO;
                }
                break;
            case StateId::fWords_q6:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::fWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::fWords_q7:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::fWords_q9;
                    ch = getNextChar();
                } else if (((ch >= 'B') && (ch <= 'Z')) || ((ch >= 'b') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::FIN;
                }
                break;
            case StateId::fWords_q9:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::fWords_q10;
                    ch = getNextChar();
                } else {
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
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q1:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::hWords_q5;
                    ch = getNextChar();
                } else if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::hWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q2:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::hWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q3:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::HAGA;
                }
                break;
            case StateId::hWords_q5:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::hWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q6:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::hWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::hWords_q7:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::HASTA;
                }
                break;
            // iWords
            case StateId::iWords_q0:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::iWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q1:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::iWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q2:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::iWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q3:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::iWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q4:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::iWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::iWords_q5:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::INICIO;
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
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q1:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::lWords_q4;
                    ch = getNextChar();
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::lWords_q3;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::lWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q11:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::LEER;
                }
                break;
            case StateId::lWords_q13:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::lWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q14:
                if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::lWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q15:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::lWords_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q16:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::lWords_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q17:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::LLAMAR;
                }
                break;
            case StateId::lWords_q2:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::LEA;
                }
                break;
            case StateId::lWords_q3:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::lWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q4:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::lWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q5:
                if ((ch == 'U') || (ch == 'u')) {
                    text += ch;
                    state = StateId::lWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q6:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::lWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q7:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::lWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::lWords_q8:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::LECTURA;
                }
                break;
            // mWords
            case StateId::mWords_q0:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::mWords_q9;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::mWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q1:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::mWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q10:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::MOD;
                }
                break;
            case StateId::mWords_q2:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::mWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q3:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::mWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q4:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::mWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::mWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q6:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::mWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::mWords_q7:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::MIENTRAS;
                }
                break;
            case StateId::mWords_q9:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::mWords_q10;
                    ch = getNextChar();
                } else {
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
                    state = StateId::id_q0;
                }
                break;
            case StateId::nWords_q1:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::nWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::nWords_q2:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::NO;
                }
                break;
            // pWords
            case StateId::pWords_q0:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::pWords_q5;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::pWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q1:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::pWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q10:
                if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::pWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q11:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::pWords_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q12:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::pWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q13:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::pWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q14:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::pWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q15:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::pWords_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q16:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::PROCEDIMIENTO;
                }
                break;
            case StateId::pWords_q2:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::pWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q3:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::PARA;
                }
                break;
            case StateId::pWords_q5:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::pWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q6:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::pWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q7:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::pWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q8:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::pWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::pWords_q9:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::pWords_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            // rWords
            case StateId::rWords_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::rWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q1:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::rWords_q2;
                    ch = getNextChar();
                } else if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::rWords_q12;
                    ch = getNextChar();
                } else if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::rWords_q5;
                    ch = getNextChar();
                } else if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::rWords_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q10:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::REGISTRO;
                }
                break;
            case StateId::rWords_q12:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::rWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q13:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::rWords_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q14:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::rWords_q15;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q15:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::REPITA;
                }
                break;
            case StateId::rWords_q17:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::rWords_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q18:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::rWords_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q19:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::rWords_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q2:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::rWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q20:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::rWords_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q21:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::RETORNE;
                }
                break;
            case StateId::rWords_q3:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::REAL;
                }
                break;
            case StateId::rWords_q5:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::rWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q6:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::rWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q7:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::rWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q8:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::rWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::rWords_q9:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::rWords_q10;
                    ch = getNextChar();
                } else {
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
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q1:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::sWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q11:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::sWords_q12;
                    ch = getNextChar();
                } else if ((ch == '_') || ((ch >= 'o') && (ch <= 'z')) || ((ch >= 'a') && (ch <= 'm')) || ((ch >= 'O') && (ch <= 'Z')) || ((ch >= 'A') && (ch <= 'M'))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::SI;
                }
                break;
            case StateId::sWords_q12:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::sWords_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q13:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::SINO;
                }
                break;
            case StateId::sWords_q2:
                if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::sWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q3:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::sWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q4:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::sWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q5:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::sWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q6:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::sWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q7:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::sWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q8:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::sWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::sWords_q9:
                if ((isalnum(ch)) || (ch == '_')) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::SECUENCIAL;
                }
                break;
            // tWords
            case StateId::tWords_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::tWords_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q1:
                if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::tWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q2:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::tWords_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::tWords_q3:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::TIPO;
                }
                break;
            // vWords
            case StateId::vWords_q0:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::vWords_q1;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::vWords_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q1:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::vWords_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q10:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::vWords_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q11:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::VERDADERO;
                }
                break;
            case StateId::vWords_q2:
                if ((ch == '_') || (isalnum(ch))) {
                    text += ch;
                    state = StateId::id_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::VAR;
                }
                break;
            case StateId::vWords_q4:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::vWords_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q5:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::vWords_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q6:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::vWords_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q7:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::vWords_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q8:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::vWords_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::id_q0;
                }
                break;
            case StateId::vWords_q9:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::vWords_q10;
                    ch = getNextChar();
                } else {
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
        case Token::OpMul: return "OpMul";
        case Token::CADENA: return "CADENA";
        case Token::charConstant: return "charConstant";
        case Token::NO: return "NO";
        case Token::CARACTER: return "CARACTER";
        case Token::OpenPar: return "OpenPar";
        case Token::DIV: return "DIV";
        case Token::OpSub: return "OpSub";
        case Token::MIENTRAS: return "MIENTRAS";
        case Token::DE: return "DE";
        case Token::SI: return "SI";
        case Token::BIN: return "BIN";
        case Token::CloseBracket: return "CloseBracket";
        case Token::ESCRITURA: return "ESCRITURA";
        case Token::OpEQ: return "OpEQ";
        case Token::ENTONCES: return "ENTONCES";
        case Token::BOOL: return "BOOL";
        case Token::ClosePar: return "ClosePar";
        case Token::Y: return "Y";
        case Token::Eol: return "Eol";
        case Token::RETORNE: return "RETORNE";
        case Token::ASSIGN: return "ASSIGN";
        case Token::OpLT: return "OpLT";
        case Token::COMMA: return "COMMA";
        case Token::REAL: return "REAL";
        case Token::Eof: return "Eof";
        case Token::ARCHIVO: return "ARCHIVO";
        case Token::ID: return "ID";
        case Token::COLON: return "COLON";
        case Token::OpenBracket: return "OpenBracket";
        case Token::stringConstant: return "stringConstant";
        case Token::NotEQ: return "NotEQ";
        case Token::OpLTE: return "OpLTE";
        case Token::ARREGLO: return "ARREGLO";
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