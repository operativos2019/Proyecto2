%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roboticPlayer.c"
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
    ;


movef:   MOVE NUMBER NUMBER 
        {move($<num>3, $<num>2);}
        ;

pick_dropf:PICK 
        {pick();}
        |
        DROP
        {drop();}
        ;
        ;

mixedf: MOVE PICK NUMBER NUMBER
        {move($<num>4, $<num>3);
        pick();}
        | MOVE DROP NUMBER NUMBER
        {move($<num>4, $<num>3);
        drop();}
        ;
%%

void yyerror(const char * s){
	printf("Error %s", s);
	exit(1);
} 


int findPythonString( const int argc, char * argv[]){
    int result = argc;
    for (int i = 0; i < argc ; i++){
        if (strcmp(argv[i] ,"-p") == 0){
            result = i + 1;
            break;
        }
    }
    return result;
}

int findConfigFile (const int argc, char * argv[]){
    int result;
    for (int i = 0; i < argc ; i++){
        if (strcmp(argv[i] ,"-c") == 0){
            result = i + 1;
            break;
        }
    }
    return result;
}

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int main(int argc, char *argv[])
{
    char * fileName, * pythonString;
    
    int configFileIndex = findConfigFile(argc, argv);
    if (configFileIndex < argc){
        fileName = argv[configFileIndex];
        yyin = fopen(fileName, "r");    
        yyparse();
    }
    else{
        int pythonStringIndex = findPythonString(argc, argv);
        if (pythonStringIndex < argc){
            pythonString = argv[pythonStringIndex];
            YY_BUFFER_STATE buffer = yy_scan_string(pythonString);
            yyparse();
            yy_delete_buffer(buffer);
        }
        else{
            return -1;
        }
    }
    return 0;
}

