#!/bin/bash
yacc -d roboticPlayer.y 
flex roboticPlayer.l 
gcc -c lex.yy.c 
gcc -c y.tab.c 
gcc lex.yy.o y.tab.o -o robot