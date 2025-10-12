//
// You might be wondering why there is a grammar file for antlr if both the lexer and parser are
// implemented manually. It was introduced early into development and is used to model complex rules
// such as nested recursive rules.
//
// This file doesn't or might not match the actual parse result nor how it is actually parsed but
// it is a good approximation of the parsing. Though the emitted tree would differ a fair bit as
// the manual parser will clean as it is parsing i.e., ComposedExpr is broken into Binary and Unary
// rules and some aspects of the parsing are ommited from the final results i.e., +(A * B) would
// just become A * B
//

grammar extended_text_mods;

options { caseInsensitive = true; }

program: composed_expr;

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
  | KW_ON
  | KW_LOG_INFO
  | KW_CREATE_MOD
  | KW_PRE
  | KW_POST
  | KW_POST_UNCONDITIONAL
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
KW_ON                : 'ON'                 ;
KW_LOG_INFO          : 'LOG_INFO'           ;
KW_CREATE_MOD        : 'CREATE_MOD'         ;
KW_PRE               : 'PRE'                ;
KW_POST              : 'POST'               ;
KW_POST_UNCONDITIONAL: 'POST_UNCONDITIONAL' ;

DOT  : '.' ;
PLUS : '+' ;
MINUS: '-' ;
STAR : '*' ;
SLASH: '/' ;

SQUOTE: '\'' ;
DQUOTE: '"'  ;

LEFT_PAREN   : '(' ;
RIGHT_PAREN  : ')' ;
LEFT_BRACKET : '[' ;
RIGHT_BRACKET: ']' ;
LEFT_CURLY   : '{' ;
RIGHT_CURLY  : '}' ;

IDENTIFIER: [A-Z_][_A-Z0-9]* ;
STRING_LITERAL: DQUOTE * DQUOTE ;

WS : [\r\n ]+ -> skip;