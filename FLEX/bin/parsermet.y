%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parsermet.h"

#define EVALUAR_FUNCION 997
#define METODO_NEWTONRAPH 998
#define METODO_PUNTOFIJO 999
#define EPSILON 1e-9

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef M_E
    #define M_E 2.71828182845904523536
#endif

void yyerror(const char *);
void yywarning(const char *);
void asignarValores(double, double, double, double);
void validar_denominador(double);

double x = 10.0, x_ant = 0.0;
double error_esperado = 0.1;
int metodo_sel = 0;

int set_x = 0;

double fx = 0.0, fdx = 0.0, gx = 0.0;

%}

%union {
    double val;
}

%type <val> expr term factor sig
%token <val> NUMERO
%token X EULER PI
%token SIN COS TAN SEC COSEC COT ASIN ACOS ATAN SINH COSH TANH
%token LOG10 LOGE EXP RAIZ

%token NT_DEC PF_DEC X_DEC FX_DEC FDX_DEC GX_DEC ERROR_DEC

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

    |
    X_DEC NUMERO FX_DEC expr '}'
    {
        metodo_sel = EVALUAR_FUNCION;
        asignarValores($2, $4, 0.0, 0.0);
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
                                    validar_denominador($3);
                                    $$ = $1 / $3;
                                }
                            }

    | term '^' factor       {   if(set_x) $$ = pow($1, $3); }

    | factor                {   if(set_x) $$ = $1;          }
    ;

factor:
    sig NUMERO                  {   if(set_x) $$ = $1 * $2;          }

    | sig X                     {   if(set_x) $$ = $1 * x;           }
    
    | sig EULER                 {   if(set_x) $$ = $1 * M_E;         }

    | sig PI                    {   if(set_x) $$ = $1 * M_PI;        }
    
    | sig '(' expr ')'          {   if(set_x) $$ = $1 * $3;          }

    | sig SIN '(' expr ')'      {   if(set_x) $$ = $1 * sin($4);     }
    
    | sig COS '(' expr ')'      {   if(set_x) $$ = $1 * cos($4);     }
    
    | sig TAN '(' expr ')'      {   if(set_x) $$ = $1 * tan($4);     }
    
    | sig COSEC '(' expr ')'    {   if(set_x){
                                        validar_denominador(sin($4));
                                        $$ = $1 * (1.0 / sin($4));
                                    }     
                                }
    
    | sig SEC '(' expr ')'      {   if(set_x){
                                        validar_denominador(cos($4));
                                        $$ = $1 * (1.0 / cos($4));
                                    }     
                                }
    
    | sig COT '(' expr ')'      {   if(set_x){
                                        validar_denominador(tan($4));
                                        $$ = $1 * (1.0 / tan($4));
                                    }     
                                }
    
    | sig ASIN '(' expr ')'     {   if(set_x) $$ = $1 * asin($4);    }

    | sig ACOS '(' expr ')'     {   if(set_x) $$ = $1 * acos($4);    }
    
    | sig ATAN '(' expr ')'     {   if(set_x) $$ = $1 * atan($4);    }

    | sig SINH '(' expr ')'     {   if(set_x) $$ = $1 * sinh($4); }
    
    | sig COSH '(' expr ')'     {   if(set_x) $$ = $1 * cosh($4); }
    
    | sig TANH '(' expr ')'     {   if(set_x) $$ = $1 * tanh($4); }

    | sig LOGE '(' expr ')'     {   if(set_x){
                                        if($4 <= 0) yyerror("Error: logaritmo natural de un número negativo o cero");
                                        else $$ = $1 * log($4);
                                    }
                                }

    | sig LOG10 '(' expr ')'    {   if(set_x){
                                        if($4 <= 0) yyerror("Error: logaritmo base 10 de un número negativo o cero");
                                        else $$ = $1 * log10($4);
                                    }
                                }

    | sig EXP '(' expr ')'      {   if(set_x) $$ = $1 * exp($4);     }
    
    | sig RAIZ '(' expr ')'     {   if(set_x){
                                        if($4 < 0.0) yyerror("Raiz negativa");
                                        else $$ = $1 * sqrt($4);
                                    }    
                                }
    ;

sig:
    '+'             { $$ = 1.0;     }
    | '-'           { $$ = -1.0;    }
    | /*Sin signo*/ { $$ = 1.0;     }
;

%%

void validar_denominador(double b){
    if(fabs(b) < EPSILON)
        yyerror("Division por cero");
    return;
}

void asignarValores(double x_metodo, double valor_f1, double valor_f2, double error_metodo){
    if(!set_x){
        x = x_metodo;
        error_esperado = error_metodo;
        set_x = 1;
        return;
    }

    if(metodo_sel == METODO_NEWTONRAPH ||  metodo_sel == EVALUAR_FUNCION){
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
    if(metodo_sel != EVALUAR_FUNCION){
        while (error > error_esperado) {
                i++;
                yyin = fopen("entrada.txt", "r");
                yyparse();
                fclose(yyin);

                if(metodo_sel == METODO_NEWTONRAPH){
                    validar_denominador(fdx);
                    x_nuevo = x - (fx / fdx);
                }
                else{
                    x_nuevo = gx;
                }

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
    }
    else{
        yyin = fopen("entrada.txt", "r");
        yyout = fopen("salida.txt", "w");
        yyparse();
        fclose(yyin);
        fclose(yyout);
        printf("  x = %lf\n", x);
        printf("  f(x) = %lf\n", fx);
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