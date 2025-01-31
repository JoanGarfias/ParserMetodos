%{
#include "parsermet_yac.h"	
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
%}

%%

"nt"[ \t\n]*"{"[ \t\n]*"x"[ \t\n]*"="   { return NT_DEC; }

"pf"[ \t\n]*"{"[ \t\n]*"x"[ \t\n]*"="   { return PF_DEC; }

"f(x)"[ \t\n]*"="                       { return FX_DEC; }

"fd(x)"[ \t\n]*"="                      { return FDX_DEC; }

"error"[ \t\n]*"="                      { return ERROR_DEC; }

"g(x)"[ \t\n]*"="                       { return GX_DEC; }

"="             { return '=';   }
";"             { return ';';   }
"("             { return '(';   }
")"             { return ')';   }
"{"             { return '{';   }
"}"             { return '}';   }
"+"             { return '+';   }
"-"             { return '-';   }
"^"             { return '^';   }
"*"             { return '*';   }
"/"             { return '/';   }
"x"             { return X;     }
"raiz"          { return RAIZ;  }
"sin"           { return SIN;   }
"cos"           { return COS;   }
"tan"           { return TAN;  }
"log"           { return LOG10; }
"ln"            { return LOGE;  }

[0-9]+(\.[0-9]*)?   { 
                        yylval.val = atof(yytext);  
                        return NUMERO;
                    }

\.[0-9]+            {
                        yylval.val = atof(yytext);
                        return NUMERO;
                    }




[ \t\n\r]+          { /* Ignorar */ }

.                   { yyerror("Token no valido"); }

%%

int yywrap() {
    return 1;
}
