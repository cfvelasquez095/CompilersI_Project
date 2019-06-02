grammar MiniLPP;

program:
    subtypes_section variable_section subprogram_decl 'inicio' statement 'EOL' 'fin'
;

subtypes_section:
    subtype_decl
    | /* epsilon */
;

subtype_decl:
    'tipo' 'ID' 'es' type 'EOL' subtype_decl
    | /* epsilon */
;

variable_section:
    variable_decl
    | /* epsilon */
;

variable_decl:
    'tipo' 'id' 'EOL' variable_decl
    | 'tipo' 'id' mult_var 'EOL'
    | /* epsilon */
;

mult_var:
    ',' 'id' mult_var
    | /* epsilon */
;

type:
    'entero'
    | 'booleano'
    | 'charConst'
    | array_type
;

array_type:
    'arreglo' '[' 'intConst' ']' 'de' type
;

subprogram_decl:
    subprogram_header 'EOL' variable_section 'inicio' statement_call 'EOL' 'fin' 'EOL' subprogram_decl
    | /* epsilon */
;

subprogram_header:
    function_header
    | procedure_header
;

function_header:
    'function' 'ID' '(' argument_decl ')' ':' type
;

procedure_header:
    'procedimiento' 'ID' '(' argument_decl ')'
;

argument_decl:
    'var' type 'ID' mult_arg
    | type 'ID' mult_arg
;

mult_arg:
    ',' 'var' type 'id' mult_arg
    | ',' type 'id' mult_arg
    | /* epsilon */
;

statement:
    lvalue '<-' expr
    | 'llamar' 'ID' '(' expr ')'
    | 'escriba' argument
    | 'lea' lvalue
    | if_statement
    | 'mientras' expr 'EOL'? 'haga' 'EOL' statement 'EOL' 'EOL' 'fin' 'mientras'
    | 'repita' 'EOL' statement 'EOL' 'EOL' 'hasta' expr
    | 'para' lvalue '<-' expr 'hasta' expr 'haga' 'EOL' statement 'EOL' 'EOL' 'fin' 'para'
;

statement_call:
    statement statement_call
    | 'EOL' statement_call
    | /* epsilon */
;

if_statement:
    'si' expr 'EOL'? 'entonces' statement 'EOL' 'EOL' else_if_block* else_block? 'fin' 'si'
;

else_if_block:
    'sino' 'si' expr 'EOL'? 'entonces' statement 'EOL' 'EOL'
;

else_block:
    'else' statement 'EOL' 'EOL'
;

argument:
    'stringConstant'
    | expr
;

lvalue:
    'ID'
    | 'ID' '[' expr ']'
;

expr:
    lvalue
    | 'ID' '(' expr mult_expr ')'
    | constant
    | expr bin_op expr
    | '-' expr
    | 'no' expr
    | '(' expr ')'
;

mult_expr:
    ',' expr
    | /* epsilon */
;

bin_op:
    arith_op
    | rel_op
    | eq_op
    | cond_op
;

arith_op:
    '+'
    | '-'
    | '*'
    | 'div'
    | 'mod'
;

rel_op:
    '<'
    | '>'
    | '<='
    | '>='
;

eq_op:
    '='
    | '<>'
;

cond_op:
    'y'
    | 'o'
;

constant:
    'intConstant'
    | 'charConstant'
    | bool_constant
;

bool_constant:
    'verdadero'
    | 'falso'
;