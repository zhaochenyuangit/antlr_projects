grammar Expr;

prog: stat+;

stat: expr NEWLINE
    | ID '=' expr NEWLINE
    | NEWLINE
    ;

expr: expr ('*'|'/') expr
    | expr ('+'|'-') expr
    | INT
    | ID
    | '(' expr ')'
    ;

ID: [a-zA-Z_0]+;
INT: [0-9]+;
NEWLINE: '\r'?'\n';
WS: [ \t]+ -> skip;
