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

int parser();
void yyerror(const char *);
void yywarning(const char *);
void asignarValores(double, double, double, double);
int validar_denominador(double);
extern int yylineno;

double x = 10.0;
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
%token ERROR_T

%left '+' '-'
%left '*' '/'
%right '^'

%start prog

%%

prog:  
    /*
    Gramatica para metodo de Newton Raphson
    P -> nt{
            x = NUMERO
            f(x) = expr
            fd(x) = expr
            error = NUMERO
        }
    Gramatica para metodo de Punto Fijo
    P -> pf{
            x = NUMERO
            f(x) = expr
            g(x) = expr
            error = NUMERO
        }
    Gramatica para evaluar funciones
    P -> {
        x = NUMERO
        f(x) = expr
        }

    Gramatica de expr
    expr ->   expr + term
            | expr - term
            | term
    
    Gramatica de term
    term ->   term * factor
            | term / factor
            | term ^ factor
            | factor
    
    Gramatica de factor
    factor ->   NUMERO 
                | X 
                | EULER
                | PI 
                | SIN 
                | COS 
                | TAN 
                | SEC 
                | COSEC 
                | COT 
                | ASIN 
                | ACOS 
                | ATAN 
                | SINH
                | COSH
                | TANH
                | LOG10
                | LOGE 
                | EXP 
                | RAIZ 
                | ( expr )
    */
    NT_DEC NUMERO FX_DEC expr FDX_DEC expr ERROR_DEC NUMERO '}'
    {
        metodo_sel = METODO_NEWTONRAPH;
        asignarValores($2, $4, $6, $8);
    }
    | PF_DEC NUMERO FX_DEC expr GX_DEC expr ERROR_DEC NUMERO '}'
    {
        metodo_sel = METODO_PUNTOFIJO;
        asignarValores($2, $4, $6, $8);
    }
    | X_DEC NUMERO FX_DEC expr '}'
    {
        metodo_sel = EVALUAR_FUNCION;
        asignarValores($2, $4, 0.0, 0.0);
    }
    | NT_DEC ERROR_T {
        yyerror("Se esperaba un numero despues de 'nt'");
        YYABORT;
    }
    |
    NT_DEC NUMERO ERROR_T {
        yyerror("Se esperaba 'f(x) = ...' despues del numero");
        YYABORT;
    }
    | NT_DEC NUMERO FX_DEC expr ERROR_T {
        yyerror("Se esperaba 'fd(x) = ...' despues de 'f(x) = ...'");
        YYABORT;
    }
    | NT_DEC NUMERO FX_DEC expr FDX_DEC expr ERROR_DEC ERROR_T {
        yyerror("Se esperaba un numero positivo para el margen de error");
        YYABORT;
    }
    | NT_DEC NUMERO FX_DEC expr FDX_DEC expr ERROR_DEC NUMERO ERROR_T {
        yyerror("Se esperaba '}' para cerrar la definicion");
        YYABORT;
    }
    | ERROR_T {
        yyerror("Se ha producido un error sintáctico inesperado");
        YYABORT;
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
                                    if(!validar_denominador($3)){
                                        yyerror("No se puede dividir por cero");
                                        YYABORT;
                                    }
                                    else{
                                        $$ = $1 / $3;
                                    }
                                }
                            }

    | term '^' factor       {   if(set_x) $$ = pow($1, $3); }

    | factor                {   if(set_x) $$ = $1;          }
    ;

factor:
    NUMERO                  {   if(set_x) $$ = $1;          }

    | sig X                     {   if(set_x) $$ = $1 * x;           }
    
    | sig EULER                 {   if(set_x) $$ = $1 * M_E;         }

    | sig PI                    {   if(set_x) $$ = $1 * M_PI;        }
    
    | sig '(' expr ')'          {   if(set_x) $$ = $1 * $3;          }

    | sig SIN '(' expr ')'      {   if(set_x) $$ = $1 * sin($4);     }
    
    | sig COS '(' expr ')'      {   if(set_x) $$ = $1 * cos($4);     }
    
    | sig TAN '(' expr ')'      {   if(set_x) $$ = $1 * tan($4);     }
    
    | sig COSEC '(' expr ')'    {   if(set_x){
                                        if(!validar_denominador(sin($4))){
                                            yyerror("Cosecante indefinida");
                                            YYABORT;
                                        }
                                        else{
                                            $$ = $1 * (1.0 / sin($4));
                                        }
                                    }     
                                }
    
    | sig SEC '(' expr ')'      {   if(set_x){
                                        if(!validar_denominador(cos($4))){
                                            yyerror("Secante no definida");
                                            YYABORT;
                                        }
                                        else{
                                            $$ = $1 * (1.0 / cos($4));
                                        }
                                    }     
                                }
    
    | sig COT '(' expr ')'      {   if(set_x){
                                        if(!validar_denominador(tan($4))){
                                            yyerror("Cotangente no definida");
                                            YYABORT;
                                        }
                                        else{
                                            $$ = $1 * (1.0 / tan($4));
                                        }
                                    }     
                                }
    
    | sig ASIN '(' expr ')'     {   if(set_x) $$ = $1 * asin($4);    }

    | sig ACOS '(' expr ')'     {   if(set_x) $$ = $1 * acos($4);    }
    
    | sig ATAN '(' expr ')'     {   if(set_x) $$ = $1 * atan($4);    }

    | sig SINH '(' expr ')'     {   if(set_x) $$ = $1 * sinh($4); }
    
    | sig COSH '(' expr ')'     {   if(set_x) $$ = $1 * cosh($4); }
    
    | sig TANH '(' expr ')'     {   if(set_x) $$ = $1 * tanh($4); }

    | sig LOGE '(' expr ')'     {   if(set_x){
                                        if($4 <= 0){
                                            yywarning("Logaritmo natural de un número negativo o cero");
                                            $$ = $1 * log($4);
                                        }
                                    }
                                }

    | sig LOG10 '(' expr ')'    {   if(set_x){
                                        if($4 <= 0){
                                            yywarning("Logaritmo base 10 de un número negativo o cero");
                                            $$ = $1 * log10($4);
                                        }
                                    }
                                }

    | sig EXP '(' expr ')'      {   if(set_x) $$ = $1 * exp($4);     }
    
    | sig RAIZ '(' expr ')'     {   if(set_x){
                                        if($4 < 0.0){
                                            yyerror("Raiz negativa. Intenta probar otros valores.");
                                            YYABORT;
                                        }
                                        else{
                                            $$ = $1 * sqrt($4);
                                        }
                                    }    
                                }
    ;

sig:
    '+'             { $$ = 1.0;     }
    | '-'           { $$ = -1.0;    }
    | /*Sin signo*/ { $$ = 1.0;     }
;

%%

int validar_denominador(double b){
    return fabs(b) < EPSILON;
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
    else{ 
        yyerror("Metodo no reconocido");
    }
}

int parser() {
    int i = 0;
    double error = 10.0;
    double x_nuevo;

    FILE *res = fopen("resultados.txt", "w");
    fclose(res);
    yyin = fopen("entrada.txt", "r");
    yyout = fopen("salida.txt", "w");
    if(yyparse() != 0){
        yyrestart(yyin);
        yyclearin;
        yylineno = 1;
        fclose(yyin);
        fclose(yyout);
        return 0;
    }
    fclose(yyin);
    fclose(yyout);

    if (metodo_sel != EVALUAR_FUNCION) {
        FILE *res = fopen("resultados.txt", "a+");
        fprintf(res, "%-10s %-12s %-12s", "Iteración", "X", "f(X)");
        if (metodo_sel == METODO_NEWTONRAPH)
            fprintf(res, " %-12s", "f'(X)");
        else
            fprintf(res, " %-12s", "g(X)");
        fprintf(res, " %-12s\n", "Error");

        // Separador visual
        fprintf(res, "---------- ------------ ------------");
        if (metodo_sel == METODO_NEWTONRAPH)
            fprintf(res, " ------------");
        else
            fprintf(res, " ------------");
        fprintf(res, " ------------\n");

        // Cerrar el archivo temporalmente
        fclose(res);

        // Iterar hasta que el error sea menor que el error esperado
        while (error > error_esperado) {
            i++;

            yyin = fopen("entrada.txt", "r");
            if(yyparse() != 0){
                yyrestart(yyin);
                yyclearin;
                yylineno = 1;
                fclose(yyin);
                fclose(yyout);
                return 0;
            }
            fclose(yyin);

            if (metodo_sel == METODO_NEWTONRAPH) {
                validar_denominador(fdx);
                x_nuevo = x - (fx / fdx);
            } else {
                x_nuevo = gx;
            }

            error = fabs(x_nuevo - x);
            x = x_nuevo;

            res = fopen("resultados.txt", "a");

            // Escribir los resultados en formato tabular
            fprintf(res, "%-10d %-12.6f %-12.6f", i, x, fx);
            if (metodo_sel == METODO_NEWTONRAPH)
                fprintf(res, " %-12.6f", fdx);
            else
                fprintf(res, " %-12.6f", gx);
            fprintf(res, " %-12.6f\n", error);

            fclose(res);
        }
    } else {
        FILE *res = fopen("resultados.txt", "w");
        yyin = fopen("entrada.txt", "r");
        yyout = fopen("salida.txt", "w");
        if(yyparse() != 0){
            yyrestart(yyin);
            yyclearin;
            yylineno = 1;
            fclose(yyin);
            fclose(yyout);
            return 0;
        }

        fprintf(res, "f(%.6f) = %.6f\n", x, fx);

        fclose(yyin);
        fclose(yyout);
        fclose(res);
    }

    x = 10.0;
    metodo_sel = 0, set_x = 0, i = 0;
    fx = 0.0, fdx = 0.0, gx = 0.0, error_esperado = 0.2;
    yylineno = 1;
    return 1;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error (Linea %d): %s\n", yylineno, s);
}

void yywarning(const char *msg) {
    fprintf(stderr, "Advertencia (Linea %d): %s\n", yylineno, msg);
}