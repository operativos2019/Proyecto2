%{
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin;
FILE * fileT;
%}
%start run
%token PICK
%token DROP
%token MOVE
%token NUMBER
%token SEPARATOR
%token EOL

%%

run: 
    function run
    |
    ;

function:
    movef
    | pick_dropf
    | mixedf
    ;


movef:   MOVE SEPARATOR NUMBER NUMBER EOL
        {printf ("%s to this location: (%d, %d)", $1, $3, $4);}

pick_dropf: PICK SEPARATOR EOL
        | DROP SEPARATOR EOL
        {printf ("%s in current location", $1);}

mixedf: MOVE SEPARATOR PICK SEPARATOR NUMBER NUMBER EOL
        | MOVE SEPARATOR DROP SEPARATOR NUMBER NUMBER EOL
    {printf ("%s and %s to this location: (%d, %d)", $1, $3, $5, $6);}
%%

int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
    return yyparse();
    printf("EOF");
    return 0;
}