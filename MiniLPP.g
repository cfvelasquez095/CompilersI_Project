grammar MiniLPP;

program: /* DONE */
    subtypes_section variable_section subprogram_decl 'inicio' statement_call 'EOL' 'fin'
;

subtypes_section: /* DONE */
    subtype_decl
    | /* epsilon */
;

subtype_decl: /* DONE */
    'tipo' 'ID' 'es' type 'EOL' subtype_decl
    | /* epsilon */
;

variable_section: /* DONE */
    variable_decl
    | /* epsilon */
;

variable_decl: /* DONE */
    'tipo' 'id' mult_var 'EOL' variable_decl
    | /* epsilon */
;

mult_var: /* DONE */
    ',' 'id' mult_var
    | /* epsilon */
;

type: /* DONE */
    'entero'
    | 'booleano'
    | 'charConst'
    | array_type
;

array_type: /* DONE */
    'arreglo' '[' 'intConst' ']' 'de' type
;

subprogram_decl: /* DONE */
    subprogram_header 'EOL' variable_section 'inicio' statement_call 'EOL' 'fin' 'EOL' subprogram_decl
    | /* epsilon */
;

subprogram_header: /* DONE */
    function_header
    | procedure_header
;

function_header: /* DONE */
    'function' 'ID' argument_call ':' type
;

procedure_header: /* DONE */
    'procedimiento' 'ID' argument_call
;

argument_decl: /* DONE */
    'var' type 'ID' mult_arg
    | type 'ID' mult_arg
;

mult_arg: /* DONE */
    ',' 'var' type 'id' mult_arg
    | ',' type 'id' mult_arg
    | /* epsilon */
;

argument_call: /* DONE */
    '(' argument_decl ')'
    | argument_decl
    | '(' ')'
;

eol_call: /* DONE */
    'EOL'
    | /* epsilon */
;

statement: /* DONE */
    lvalue '<-' expr
    | 'llamar' 'ID' '(' expr ')'
    | 'escriba' argument
    | 'lea' lvalue
    | 'si' expr eol_call 'entonces' statement_call 'EOL' else_if_block else_block 'fin' 'si'
    | 'mientras' expr eol_call 'haga' 'EOL' statement_call 'EOL' 'fin' 'mientras'
    | 'repita' 'EOL' statement_call 'EOL' 'hasta' expr
    | 'para' lvalue '<-' expr 'hasta' expr 'haga' 'EOL' statement_call 'EOL' 'fin' 'para'
;

statement_call: /* DONE */
    statement 'EOL' statement_call
    | /* epsilon */
;

else_if_block: /* DONE */
    'sino' 'si' expr eol_call 'entonces' statement_call 'EOL'
    | /* epsilon */
;

else_block: /* DONE */
    'sino' statement_call 'EOL'
    | /* epsilon */
;

argument: /* DONE */
    'stringConstant'
    | expr
;

lvalue: /* DONE */
    'ID'
    | 'ID' '[' expr ']'
;

expr:
    lvalue
    | 'ID' expr_call
    | constant
    | expr bin_op expr
    | '-' expr
    | 'no' expr
    | '(' expr ')'
    | /* epsilon */
;

mult_expr: /* DONE */
    ',' expr
    | /* epsilon */
;

expr_call: /* DONE */
    '(' expr mult_expr ')'
    | expr mult_expr
    | '(' ')'
    | /* epsilon */
;

bin_op: /* DONE */
    arith_op
    | rel_op
    | eq_op
    | cond_op
;

arith_op: /* DONE */
    '+'
    | '-'
    | '*'
    | 'div'
    | 'mod'
;

rel_op: /* DONE */
    '<'
    | '>'
    | '<='
    | '>='
;

eq_op: /* DONE */
    '='
    | '<>'
;

cond_op: /* DONE */
    'y'
    | 'o'
;

constant: /* DONE */
    'intConstant'
    | 'charConstant'
    | bool_constant
;

bool_constant: /* DONE */
    'verdadero'
    | 'falso'
;