grammar sysY;

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


// integer, identifier
IntLiteral
    :  Digit
    | '0x'[0-9a-fA-F]+
    | '0X'[0-9a-fA-F]+
    ;

Identifier
    : [a-zA-Z_][a-zA-Z_0-9]*
    ;

FloatLiteral
    : Digit '.' Digit*
    |       '.' Digit+
    ;

fragment
Digit: [0-9]+;

STRING : '"'(ESC|.)*?'"';

fragment
ESC : '\\"'|'\\\\';

WS : 
    [ \t\r\n] -> skip
    ;

LINE_COMMENT : '//' .*? '\r'? '\n' -> skip;
COMMENT      :'/*'.*?'*/'-> skip ;

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
    : exp # scalarInitVal
    | '{' (initVal (',' initVal)* )? '}' # listInitval
    ;

funcDef : funcType Identifier '(' (funcFParams)? ')' block;

funcType : 'void' | 'int';

funcFParams : funcFParam (',' funcFParam)*;

funcFParam : bType Identifier ('[' ']' ('[' constExp ']')* )?;

block : '{' (blockItem)* '}';

blockItem : decl | stmt;

stmt
    : lVal '=' exp ';' # assignment
    | (exp)? ';' # expStmt
    | block # blockStmt
    | 'if' '(' cond ')' stmt # ifStmt1
    | 'if' '(' cond ')' stmt 'else' stmt # ifStmt2
    | 'while' '(' cond ')' stmt # whileStmt
    | 'break' ';' # breakStmt
    | 'continue' ';' # continueStmt
    | 'return' (exp)? ';' # returnStmt
    ;

exp : addExp;

cond : lOrExp;

lVal : Identifier ('[' exp ']')*;

primaryExp
    : '(' exp ')' # primaryExp1
    | lVal # primaryExp2
    | number # primaryExp3
    ;

number : IntLiteral
       | FloatLiteral;

unaryExp
    : primaryExp # unary1
    | Identifier '(' (funcRParams)? ')' # unary2
    | unaryOp unaryExp # unary3
    ;

unaryOp : '+' | '-' | '!';

funcRParams : funcRParam (',' funcRParam)*;

funcRParam
    : exp # expAsRParam
    | STRING # stringAsRParam
    ;

mulExp
    : unaryExp # mul1
    | mulExp ('*' | '/' | '%') unaryExp # mul2
    ;

addExp
    : mulExp # add1
    | addExp ('+' | '-') mulExp # add2
    ;

relExp
    : addExp # rel1
    | relExp ('<' | '>' | '<=' | '>=') addExp # rel2
    ;
eqExp
    : relExp # eq1
    | eqExp ('==' | '!=') relExp # eq2
    ;

lAndExp
    : eqExp # lAnd1
    | lAndExp '&&' eqExp # lAnd2
    ;

lOrExp
    : lAndExp # lOr1
    | lOrExp '||' lAndExp # lOr2
    ;

constExp
    : addExp
    ;
