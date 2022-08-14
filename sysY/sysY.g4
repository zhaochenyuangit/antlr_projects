grammar sysY;
// Here starts the Lexer
// keyword
Int : 'int';
Float: 'float';
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

ADD : '+';
SUB : '-';
MUL : '*';
DIV : '/';
MOD : '%';

NOT : '!';
AND : '&&';
OR : '||';
EQ : '==';
NEQ : '!=';
LT : '<';
LE : '<=';
GT : '>';
GE : '>=';

Tilde : '~';


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

bType: type=('int'|'float');

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
    : lVal '=' arithExp ';'     # assignStmt // assignment in sysY do not return a value
    | (arithExp)? ';'           # expStmt
    | block                     # blockStmt
    | 'if' '(' condExp ')' stmt # ifStmt
    | 'if' '(' condExp ')' stmt 'else' stmt # ifElseStmt
    | 'while' '(' condExp ')' stmt          # whileStmt
    | 'break' ';'               # breakStmt
    | 'continue' ';'            # continueStmt
    | 'return' (arithExp)? ';'  # returnStmt
    ;

lVal : Identifier ('[' arithExp ']')*;

constNumber 
    : IntConst # intNumber
    | FloatConst #floatNumber;

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
    : arithExp  # expAsRParam
    | STRING    # stringAsRParam
    ;

primaryExp
    : lVal              # primaryLVal
    | constNumber       # primaryNumber
    | '(' arithExp ')'  # primaryParen
    ;

unaryExp
    : primaryExp                        # unaryPrimary
    | op=('+' | '-' | '!') unaryExp        # unaryOp
    | Identifier '(' (funcRParams)? ')' # unaryFuncCall
    ;

// distinguish arithExp from condExp because the sysY language reference
// states that function-returned values must be int or float
// and initValue, returnValue, constValue can only be arithmetic values
arithExp
    : unaryExp                              # arithUnary
    | arithExp  op=('*' | '/' | '%') unaryExp  # arithMul
    | arithExp op=('+' | '-') arithExp         # arithAdd
    ;

condExp
    : arithExp                                   # condArith
    | condExp op=('<' | '>' | '<=' | '>=') arithExp # condCompare
    | condExp op=('==' | '!=') condExp              # condEqual
    | condExp '&&' condExp                      # condAnd
    | condExp'||' condExp                       # condOr
    ;

constExp // constExp must be a compile-time arith constant
    : arithExp
    ;
