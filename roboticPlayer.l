%{
#include <stdio.h>
#include "y.tab.h"
%}
%option yylineno

%%
 
move return MOVE;
drop return DROP;
pick return PICK;
[0-9] {yylval.num = atoi(yytext); return NUMBER;};
%%

int yywrap(void){  
    return 1; 
    }