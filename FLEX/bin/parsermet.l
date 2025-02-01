%{
#include <stdlib.h>
#include <string.h>
#include "parsermetTok.h"

void yyerror(const char *s);
%}

%%

"nt"[ \t\n]*"{"[ \t\n]*"x"[ \t\n]*"="   { return NT_DEC;    }

"pf"[ \t\n]*"{"[ \t\n]*"x"[ \t\n]*"="   { return PF_DEC;    }

"f(x)"[ \t\n]*"="                       { return FX_DEC;    }

"fd(x)"[ \t\n]*"="                      { return FDX_DEC;   }

"error"[ \t\n]*"="                      { return ERROR_DEC; }

"g(x)"[ \t\n]*"="                       { return GX_DEC;    }

[ \t\n]*"{"[ \t\n]*"x"[ \t\n]*"="       { return X_DEC;     }

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

"sin"           { return SIN;   }
"cos"           { return COS;   }
"tan"           { return TAN;   }
"cosec"         { return COSEC; }
"sec"           { return SEC;   }
"cot"           { return COT;   }

"asin"          { return ASIN;  }
"acos"          { return ACOS;  }
"atan"          { return ATAN;  }
"sinh"          { return SINH;  }
"cosh"          { return COSH;  }
"tanh"          { return TANH;  }


"log"           { return LOG10; }
"ln"            { return LOGE;  }
"exp"           { return EXP;   }
"raiz"          { return RAIZ;  }

"e"             { return EULER; }
"pi"            { return PI;    }

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
