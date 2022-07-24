grammar sysY;
// Here starts the Lexer
// keyword
Int : 'int';
Void: 'void';
Const: 'const';
Return : 'return';
If : 'if';
Else : 'else';
For : 'for';
While : 'while';
Do : 'do';
Break : 'break';
Continue : 'continue'; 

// operator
Lparen : '(' ;
Rparen : ')' ;
Lbrkt : '[' ;
Rbrkt : ']' ;
Lbrace : '{' ;
Rbrace : '}' ;
Comma : ',' ;
Semicolon : ';';
Question : '?';
Colon : ':';

Minus : '-';
Exclamation : '!';
Tilde : '~';
Addition : '+';
Multiplication : '*';
Division : '/';
Modulo : '%';
LAND : '&&';
LOR : '||';
EQ : '==';
NEQ : '!=';
LT : '<';
LE : '<=';
GT : '>';
GE : '>=';

// literals
IntConst
    :  Digit
    | '0x'[0-9a-fA-F]+
    | '0X'[0-9a-fA-F]+
    ;

FloatConst
    : Digit '.' Digit*
    |       '.' Digit+
    ;

Identifier
    : [a-zA-Z_][a-zA-Z_0-9]*
    ;


fragment
Digit: [0-9]+;

STRING : '"'(ESC|.)*?'"';

fragment
ESC : '\\"'|'\\\\';

// skip the comments and blanks
WS : 
    [ \t\r\n] -> skip
    ;

LINE_COMMENT : '//' .*? '\r'? '\n' -> skip;
COMMENT      :'/*'.*?'*/'-> skip ;


// Here starts the Parser
compUnit : (decl | funcDef)* EOF;

decl : constDecl | varDecl;

constDecl : 'const' bType constDef (',' constDef)* ';' ;

bType: 'int'
     | 'float';

constDef : Identifier ('[' constExp ']')* '=' constInitVal;

constInitVal
    : constExp # scalarConstInitVal
    | '{' (constInitVal (',' constInitVal)* )? '}' # listConstInitVal
    ;

varDecl : bType varDef (',' varDef)* ';';

varDef
    : Identifier ('[' constExp ']')* # uninitVarDef
    | Identifier ('[' constExp ']')* '=' initVal # initVarDef
    ;

initVal
    : arithExp # scalarInitVal
    | '{' (initVal (',' initVal)* )? '}' # listInitval
    ;

funcDef : funcType Identifier '(' (funcFParams)? ')' block;

funcType : 'void' | 'int' | 'float';

funcFParams : funcFParam (',' funcFParam)*;

// for array as param, the length of first dim is ommited, written as [] literal 
funcFParam : bType Identifier ('[' ']' ('[' constExp ']')* )?; 

block : '{' (blockItem)* '}';

blockItem : decl | stmt;

stmt
    : lVal '=' arithExp ';' # assignment // assignment in sysY do not return a value
    | (arithExp)? ';' # expStmt
    | block # blockStmt
    | 'if' '(' condExp ')' stmt # ifStmt1
    | 'if' '(' condExp ')' stmt 'else' stmt # ifStmt2
    | 'while' '(' condExp ')' stmt # whileStmt
    | 'break' ';' # breakStmt
    | 'continue' ';' # continueStmt
    | 'return' (arithExp)? ';' # returnStmt
    ;

lVal : Identifier ('[' arithExp ']')*;

number : IntConst    # number1
       | FloatConst  # number2
       ;

// expression priority
// unary : + - !
// mul: * / %
// add: + -
// relation: < > <= >=
// eq: == !=
// and: &&
// or: ||

funcRParams : funcRParam (',' funcRParam)*;

funcRParam
    : arithExp # expAsRParam
    | STRING # stringAsRParam
    ;

unaryExp
    : lVal # unary1
    | number # unary2
    | '(' arithExp ')' # unary3
    | Identifier '(' (funcRParams)? ')' # unary4
    | ('+' | '-' | '!') unaryExp # unary5
    ;

// distinguish arithExp from condExp because the sysY language reference
// states that function-returned values must be int or float
// and initValue, returnValue, constValue can only be arithmetic values
arithExp
    : unaryExp # arith1
    | arithExp  ('*' | '/' | '%') unaryExp # arith2
    | arithExp ('+' | '-') arithExp # arith3
    ;

condExp
    : arithExp  # cond1
    | condExp ('<' | '>' | '<=' | '>=') arithExp # cond2
    | condExp ('==' | '!=') condExp # cond3
    | condExp '&&' condExp # cond4
    | condExp'||' condExp # cond5
    ;

constExp // constExp must be a compile-time arith constant
    : arithExp
    ;
