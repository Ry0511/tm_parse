grammar extended_text_mods;

options { caseInsensitive = true; }

program
  : ( set_command
    | object_definition
    )*
    EOF
  ;

identifier
  : IDENTIFIER
  | KW_TRUE
  | KW_FALSE
  | KW_NONE
  | KW_SET
  | KW_LEVEL
  | KW_PACKAGE
  | KW_BEGIN
  | KW_OBJECT
  | KW_CLASS
  | KW_NAME
  | KW_END
  | KW_BEFORE
  | KW_AFTER
  | KW_LOG_INFO
  | KW_CREATE_MOD
  ;

literal_expr
  : identifier
  | NUMBER
  | STRING_LITERAL
  | KW_TRUE
  | KW_FALSE
  | KW_NONE
  ;

// -------------------------------------------------------------------------------------------------
// -- COMMON RULES --
// -------------------------------------------------------------------------------------------------

meta_var
  : DOLLAR_SIGN LEFT_PAREN prop_dot_identifier RIGHT_PAREN
  ;

static_array_access
  : LEFT_BRACKET NUMBER RIGHT_BRACKET
  ;

dynamic_array_access
  : LEFT_PAREN NUMBER RIGHT_PAREN
  ;

array_access
  : ( static_array_access | dynamic_array_access )+
  ;

obj_dot_identifier
  : identifier ( DOT identifier )*
  ;

prop_dot_identifier
  : identifier array_access? ( DOT identifier array_access? )*
  ;

property_access
  : prop_dot_identifier
  ;

class_ref
  : identifier SQUOTE obj_dot_identifier SQUOTE
  ;

full_object_ref
  : obj_dot_identifier ( COLON obj_dot_identifier )?
  ;

object_reference
  : class_ref
  | full_object_ref
  | meta_var
  ;

expression
  : meta_var
  | paren_expr
  | assignment_expr_list
  | assignment_expr
  | composed_expr
  | literal_expr
  | identifier
  ;

// -------------------------------------------------------------------------------------------------
// -- SET COMMANDS --
// -------------------------------------------------------------------------------------------------

// NOTE: set foo baz(0)(1) succeeds here but will fail in the actual implementation since array
//  access is guaranteed to be greedy
set_command
  : KW_SET object_reference property_access expression
  ;

object_definition
  : KW_BEGIN KW_OBJECT
    KW_CLASS EQUAL obj_dot_identifier
    KW_NAME EQUAL obj_dot_identifier
    ( object_definition
    | assignment_expr
    )*
    KW_END KW_OBJECT
  ;

// -------------------------------------------------------------------------------------------------
// -- EXPRESSIONS --
// -------------------------------------------------------------------------------------------------

paren_expr
  : LEFT_PAREN expression RIGHT_PAREN
  ;

assignment_expr
  : property_access EQUAL expression
  ;

assignment_expr_list
  : LEFT_PAREN assignment_expr ( COMMA assignment_expr )* RIGHT_PAREN
  ;

// -------------------------------------------------------------------------------------------------
// -- MATHS EXPRESSIONS --
// -------------------------------------------------------------------------------------------------

composed_expr
  : composed_expr_term
    ( ( PLUS | MINUS ) composed_expr_term )*
  ;

composed_expr_term
  : composed_expr_factor
    ( ( STAR | SLASH ) composed_expr_factor )*
  ;

composed_expr_factor
  : literal_expr
  | composed_expr_paren
  | composed_expr_unary
  ;

composed_expr_unary
  : ( PLUS | MINUS ) composed_expr_factor
  ;

composed_expr_paren
  : LEFT_PAREN composed_expr RIGHT_PAREN
  ;

// -------------------------------------------------------------------------------------------------
// -- LEXER STUFF --
// -------------------------------------------------------------------------------------------------

NUMBER: (MINUS | PLUS)? [0-9]+ ( DOT [0-9]+ )?;

KW_TRUE              : 'TRUE'               ;
KW_FALSE             : 'FALSE'              ;
KW_NONE              : 'NONE'               ;
KW_SET               : 'SET'                ;
KW_LEVEL             : 'LEVEL'              ;
KW_PACKAGE           : 'PACKAGE'            ;
KW_BEGIN             : 'BEGIN'              ;
KW_OBJECT            : 'OBJECT'             ;
KW_CLASS             : 'CLASS'              ;
KW_NAME              : 'NAME'               ;
KW_END               : 'END'                ;
KW_LOG_INFO          : 'LOG_INFO'           ;
KW_CREATE_MOD        : 'CREATE_MOD'         ;
KW_BEFORE            : 'BEFORE'             ;
KW_AFTER             : 'AFTER'              ;

DOT  : '.' ;
PLUS : '+' ;
MINUS: '-' ;
STAR : '*' ;
SLASH: '/' ;

SQUOTE: '\'' ;
DQUOTE: '"'  ;

COMMA        : ',' ;
EQUAL        : '=' ;
DOLLAR_SIGN  : '$' ;
LEFT_PAREN   : '(' ;
RIGHT_PAREN  : ')' ;
LEFT_BRACKET : '[' ;
RIGHT_BRACKET: ']' ;
LEFT_CURLY   : '{' ;
RIGHT_CURLY  : '}' ;
COLON        : ':' ;

IDENTIFIER: [A-Z_][_A-Z0-9]* ;

STRING_LITERAL: DQUOTE * DQUOTE ;

WS : [\t\r\n ]+ -> skip;