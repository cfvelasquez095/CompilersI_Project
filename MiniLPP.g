grammar MiniLPP;

program:
    subtypes_section variable_section subprogram_decl 'inicio' statement 'EOL' 'fin'
;

subtypes_section:
    subtype_decl 'EOL'
;

subtype_decl:
    'tipo' 'ID' 'es' type subtype_decl
    | 'EOL' subtype_decl
    | /* epsilon */
;

variable_section:
    variable_decl 'EOL'
;

variable_decl:
    'tipo' 'id' variable_decl
    | 'tipo' 'id' mult_var
    | 'EOL' variable_decl
;

mult_var:
    'colon' 'id' mult_var
    | /* epsilon */
;

type:
    'entero'
    | 'booleano'
    | 'charConst'
    | array_type
;

array_type:
    'arreglo' '[' 'intConst' '..' 'intConst' ']' 'de' type
;

subprogram_decl:
;

statement:
;