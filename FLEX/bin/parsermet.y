%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parsermet.h"

#define METODO_NEWTONRAPH 998
#define METODO_PUNTOFIJO 999

void yyerror(const char *s);
void yywarning(const char *msg);
void asignarValores(double x, double vf1, double vf2, double err);

double x = 10.0, x_ant = 0.0;
double error_esperado = 0.1;
int metodo_sel = 0;

int set_x = 0;

double fx = 0.0, fdx = 0.0, gx = 0.0;

%}

%union {
    double val;
}

%type <val> expr term factor
%token <val> NUMERO
%token X
%token SIN COS TAN LOG10 LOGE RAIZ
%token NT_DEC PF_DEC FX_DEC FDX_DEC ERROR_DEC GX_DEC

%left '+' '-'
%left '*' '/'
%right '^'

%start prog

%%

prog:    
    NT_DEC NUMERO FX_DEC expr FDX_DEC expr ERROR_DEC NUMERO '}'
    {
        metodo_sel = METODO_NEWTONRAPH;
        asignarValores($2, $4, $6, $8);
    }
    |
    PF_DEC NUMERO FX_DEC expr GX_DEC expr ERROR_DEC NUMERO '}'
    {
        metodo_sel = METODO_PUNTOFIJO;
        asignarValores($2, $4, $6, $8);
    }
    ;

expr:
    expr '+' term           {   if(set_x) $$ = $1 + $3;     }

    | expr '-' term         {   if(set_x) $$ = $1 - $3;     }

    | term                  {   if(set_x) $$ = $1;  }
    ;

term:
    term '*' factor         {   if(set_x) $$ = $1 * $3;     }

    | term '/' factor       {
                                if(set_x){
                                    if ($3 == 0) {
                                        yyerror("División por cero no permitida.");
                                    } else {
                                        $$ = $1 / $3;
                                    }
                                }
                            }

    | term '^' factor       {   if(set_x) $$ = pow($1, $3); }

    | factor                {   if(set_x) $$ = $1;          }
    ;

factor:
    NUMERO                  {   if(set_x) $$ = $1;          }

    | X                     {   if(set_x) $$ = x;           }
    
    | '(' expr ')'          {   if(set_x) $$ = $2;          }
    
    | SIN '(' expr ')'      {   if(set_x) $$ = sin($3);     }
    
    | COS '(' expr ')'      {   if(set_x) $$ = cos($3);     }
    
    | TAN '(' expr ')'      {   if(set_x) $$ = tan($3);     }
    
    | LOG10 '(' expr ')'    {   if(set_x) $$ = log10($3);   }
    
    | LOGE '(' expr ')'     {   if(set_x) $$ = log($3);     }
    
    | RAIZ '(' expr ')'     {   if(set_x) $$ = sqrt($3);    }
    ;

%%

void asignarValores(double x_metodo, double valor_f1, double valor_f2, double error_metodo){
    if(!set_x){
        x = x_metodo;
        error_esperado = error_metodo;
        set_x = 1;
        return;
    }

    if(metodo_sel == METODO_NEWTONRAPH){
        fx = valor_f1;
        fdx = valor_f2;
    }
    else if(metodo_sel == METODO_PUNTOFIJO){
        fx = valor_f1;
        gx = valor_f2;
    }
    else{ yyerror("Metodo no reconocido");}
}

int main() {
    int i = 0;
    double error = 10.0;
    double x_nuevo;

    yyin = fopen("entrada.txt", "r");
    yyout = fopen("salida.txt", "w");
    yyparse();
    fclose(yyin);
    fclose(yyout);

    while (error > error_esperado) {
            i++;
            yyin = fopen("entrada.txt", "r");
            yyparse();
            fclose(yyin);

            if(metodo_sel == METODO_NEWTONRAPH)
                x_nuevo = x - (fx / fdx);
            else
                x_nuevo = gx;

            error = fabs(x_nuevo - x);
            x = x_nuevo;

            printf("Iteracion %d:\n", i);
            printf("  x = %lf\n", x);
            printf("  Error = %lf\n", error);
            printf("  f(x) = %lf\n", fx);
            if(metodo_sel == METODO_NEWTONRAPH)
                printf("  f'(x) = %lf\n", fdx);
            else
                printf("  g(x) = %lf\n", gx);
    }


    x = 10.0, x_ant = 0.0;
    metodo_sel = 0, set_x = 0, i = 0;
    fx = 0.0, fdx = 0.0, gx = 0.0, error_esperado = 0.2;

    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

void yywarning(const char *msg) {
    fprintf(stderr, "Advertencia: %s\n", msg);
}