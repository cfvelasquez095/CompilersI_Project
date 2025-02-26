#ifndef __LEXER_H__
#define __LEXER_H__

enum class StateId {
    Start_q0,
    Start_q1,
    Start_q17,
    Start_q2,
    Start_q7,
    intConst_q0,
    intConst_q1,
    intConst_q2,
    intConst_q3,
    intConst_q4,
    charConst_q0,
    charConst_q1,
    charConst_q3,
    strConst_q0,
    strConst_q1,
    strConst_q3,
    strConst_q4,
    lcomm_q0,
    lcomm_q1,
    bcomm_q0,
    bcomm_q1,
    bcomm_q2,
    operators_q0,
    operators_q4,
    operators_q5,
    resWords_q0,
    resWords_q12,
    resWords_q18,
    aWords_q0,
    aWords_q1,
    aWords_q10,
    aWords_q11,
    aWords_q12,
    aWords_q13,
    aWords_q14,
    aWords_q15,
    aWords_q2,
    aWords_q3,
    aWords_q4,
    aWords_q5,
    aWords_q6,
    aWords_q7,
    aWords_q8,
    aWords_q9,
    bWords_q0,
    bWords_q1,
    bWords_q2,
    bWords_q3,
    bWords_q4,
    bWords_q5,
    bWords_q6,
    bWords_q7,
    cWords_q0,
    cWords_q1,
    cWords_q10,
    cWords_q11,
    cWords_q12,
    cWords_q13,
    cWords_q14,
    cWords_q15,
    cWords_q16,
    cWords_q17,
    cWords_q18,
    cWords_q19,
    cWords_q2,
    cWords_q20,
    cWords_q21,
    cWords_q3,
    cWords_q4,
    cWords_q5,
    cWords_q6,
    cWords_q7,
    cWords_q8,
    cWords_q9,
    dWords_q0,
    dWords_q1,
    dWords_q3,
    dWords_q4,
    eWords_q0,
    eWords_q1,
    eWords_q10,
    eWords_q11,
    eWords_q13,
    eWords_q14,
    eWords_q15,
    eWords_q16,
    eWords_q17,
    eWords_q18,
    eWords_q19,
    eWords_q2,
    eWords_q20,
    eWords_q21,
    eWords_q22,
    eWords_q23,
    eWords_q24,
    eWords_q3,
    eWords_q4,
    eWords_q5,
    eWords_q7,
    eWords_q8,
    eWords_q9,
    fWords_q0,
    fWords_q1,
    fWords_q10,
    fWords_q12,
    fWords_q13,
    fWords_q14,
    fWords_q15,
    fWords_q16,
    fWords_q17,
    fWords_q2,
    fWords_q3,
    fWords_q4,
    fWords_q6,
    fWords_q7,
    fWords_q9,
    hWords_q0,
    hWords_q1,
    hWords_q2,
    hWords_q3,
    hWords_q5,
    hWords_q6,
    hWords_q7,
    iWords_q0,
    iWords_q1,
    iWords_q2,
    iWords_q3,
    iWords_q4,
    iWords_q5,
    lWords_q0,
    lWords_q1,
    lWords_q11,
    lWords_q13,
    lWords_q14,
    lWords_q15,
    lWords_q16,
    lWords_q17,
    lWords_q2,
    lWords_q3,
    lWords_q4,
    lWords_q5,
    lWords_q6,
    lWords_q7,
    lWords_q8,
    mWords_q0,
    mWords_q1,
    mWords_q10,
    mWords_q2,
    mWords_q3,
    mWords_q4,
    mWords_q5,
    mWords_q6,
    mWords_q7,
    mWords_q9,
    nWords_q0,
    nWords_q1,
    nWords_q2,
    pWords_q0,
    pWords_q1,
    pWords_q10,
    pWords_q11,
    pWords_q12,
    pWords_q13,
    pWords_q14,
    pWords_q15,
    pWords_q16,
    pWords_q2,
    pWords_q3,
    pWords_q5,
    pWords_q6,
    pWords_q7,
    pWords_q8,
    pWords_q9,
    rWords_q0,
    rWords_q1,
    rWords_q10,
    rWords_q12,
    rWords_q13,
    rWords_q14,
    rWords_q15,
    rWords_q17,
    rWords_q18,
    rWords_q19,
    rWords_q2,
    rWords_q20,
    rWords_q21,
    rWords_q3,
    rWords_q5,
    rWords_q6,
    rWords_q7,
    rWords_q8,
    rWords_q9,
    sWords_q0,
    sWords_q1,
    sWords_q11,
    sWords_q12,
    sWords_q13,
    sWords_q2,
    sWords_q3,
    sWords_q4,
    sWords_q5,
    sWords_q6,
    sWords_q7,
    sWords_q8,
    sWords_q9,
    tWords_q0,
    tWords_q1,
    tWords_q2,
    tWords_q3,
    vWords_q0,
    vWords_q1,
    vWords_q10,
    vWords_q11,
    vWords_q2,
    vWords_q4,
    vWords_q5,
    vWords_q6,
    vWords_q7,
    vWords_q8,
    vWords_q9,
    id_q0,
    id_q1,
};

enum class Token {
    VERDADERO,
    REPITA,
    REGISTRO,
    PROCEDIMIENTO,
    MOD,
    LEA,
    SECUENCIAL,
    LLAMAR,
    LEER,
    INICIO,
    HASTA,
    HAGA,
    FIN,
    FALSO,
    SINO,
    FUNCION,
    FINAL,
    VAR,
    LECTURA,
    ESCRIBIR,
    ESCRIBA,
    ES,
    DECIMAL,
    OpMul,
    CADENA,
    charConstant,
    NO,
    CARACTER,
    OpenPar,
    DIV,
    OpSub,
    MIENTRAS,
    DE,
    SI,
    BIN,
    CloseBracket,
    ESCRITURA,
    OpEQ,
    ENTONCES,
    BOOL,
    ClosePar,
    Y,
    Eol,
    RETORNE,
    ASSIGN,
    OpLT,
    COMMA,
    REAL,
    Eof,
    ARCHIVO,
    ID,
    COLON,
    OpenBracket,
    stringConstant,
    NotEQ,
    OpLTE,
    ARREGLO,
    ENTERO,
    HEX,
    OpGTE,
    PARA,
    OpGT,
    ABRIR,
    TIPO,
    CASO,
    COMO,
    OpAdd,
    O,
    CERRAR,
};
#endif