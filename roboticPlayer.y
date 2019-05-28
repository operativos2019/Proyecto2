%{
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin;
FILE * fileT;
int yylex();
void yyerror(const char *s);
%}

%union {int num;};

%start RUN
%token PICK
%token DROP
%token MOVE
%token <num> NUMBER

%%

RUN:
    function RUN
    | 
    ;

function:
    movef
    | pick_dropf
    | mixedf
    | NUMBER {printf("Number is: %d\n", $1);};
    ;


movef:   MOVE NUMBER NUMBER 
        {printf ("move to this location: (%d, %d)", $<num>2, $<num>3);}
        ;

pick_dropf:PICK NUMBER NUMBER 
        {printf ("pick to this location: (%d, %d)", $<num>2, $<num>3);}
        |
        DROP NUMBER NUMBER 
        {printf ("drop to this location: (%d, %d)", $<num>2, $<num>3);}
        ;
        ;

mixedf: MOVE PICK NUMBER NUMBER NUMBER NUMBER
        {printf ("Move pick from this location: (%d, %d) to (%d, %d)", $<num>3, $<num>4, $<num>5, $<num>6);}
        | MOVE DROP NUMBER NUMBER NUMBER NUMBER
        {printf ("Move drop from this location: (%d, %d) to (%d, %d)", $<num>3, $<num>4, $<num>5, $<num>6);}
        ;
%%

void yyerror(const char * s){
	printf("Error %s", s);
	exit(1);
} 


int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}

