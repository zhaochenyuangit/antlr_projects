grammar Expr;

ID: [a-zA-Z_0]+;
INT: [0-9]+;
NEWLINE: '\r'?'\n';
WS: [ \t]+ -> skip;
MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';


prog: stat+;

stat: expr NEWLINE          # printExpr
    | ID '=' expr NEWLINE   # assign
    | NEWLINE               # blank
    ;

expr: expr op=('*'|'/') expr  # mul
    | expr op=('+'|'-') expr  # add
    | INT                   # int
    | ID                    # id
    | '(' expr ')'          # parens
    ;


