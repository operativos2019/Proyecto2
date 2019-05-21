%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
 
move return MOVE;
drop return DROP;
pick return PICK;
[0-9] return NUMBER;
- return EOL;
: return SEPARATOR;
%%