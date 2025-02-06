# README

### PARSER DE MÉTODOS NUMÉRICOS REALIZADO EN FLEX Y BISON
![dodecaedro](https://github.com/user-attachments/assets/c05890de-a246-4cca-9e3b-f0ee75e695b4)

## Descripción del Proyecto

Este proyecto implementa un analizador sintáctico y léxico utilizando **Bison** y **Flex** para resolver ecuaciones no lineales mediante dos métodos numéricos: **Método de Newton-Raphson** y **Método de Punto Fijo**. El código está diseñado para leer una entrada desde un archivo de texto (`entrada.txt`), procesarla, y luego escribir los resultados en otro archivo de texto (`salida.txt`).

---

## Estructura del Código

El código está dividido en varias secciones principales:

### 1. **Definiciones y Declaraciones**
- Se incluyen las bibliotecas necesarias y se definen constantes para los métodos numéricos (`METODO_NEWTONRAPH` y `METODO_PUNTOFIJO`).
- Se declaran variables globales como `x`, `fx`, `fdx`, `gx`, y `error_esperado`, que se utilizan para almacenar los valores durante el proceso de iteración.
- Se definen los tokens y las reglas gramaticales para el analizador sintáctico.

### 2. **Reglas Gramaticales**
- Las reglas gramaticales definen cómo se deben interpretar las expresiones matemáticas. Estas reglas incluyen operaciones básicas como suma, resta, multiplicación, división, y funciones matemáticas como seno, coseno, tangente, logaritmo y raíz cuadrada.
- Las reglas también manejan la asignación de valores a las variables `x`, `fx`, `fdx`, y `gx` dependiendo del método seleccionado.

### 3. **Funciones de Asignación y Error**
- `asignarValores`: Esta función asigna los valores iniciales para `x`, `fx`, `fdx`, `gx`, y `error_esperado` dependiendo del método seleccionado.
- `yyerror` y `yywarning`: Estas funciones manejan los errores y advertencias durante el análisis sintáctico.

### 4. **Función Principal (`parser()`)**
- La función principal lee la entrada desde `entrada.txt`, realiza el análisis sintáctico, y luego ejecuta el método numérico seleccionado (Newton-Raphson o Punto Fijo) para encontrar la raíz de la ecuación.
- El proceso se repite hasta que el error sea menor que el error esperado.
- Los resultados de cada iteración se imprimen en el archivo `resultados.txt`.

---

## Métodos Numéricos Implementados

### Método de Newton-Raphson
El método de Newton-Raphson es un algoritmo iterativo para encontrar la raíz de una función. Se basa en la siguiente fórmula:
```math
x_{nuevo} = x - \frac{f(x)}{f'(x)}
```

Donde:
- _x_ es el valor actual.
- _f(x)_ es el valor de la función en _x_.
- _f'(x)_ es la derivada de la función en _x_.

### Método de Punto Fijo
El método de Punto Fijo es otro algoritmo iterativo que encuentra la raíz de una función transformando la ecuación original _f(x) = 0_ en una ecuación de la forma _x = g(x)_. La iteración se realiza mediante:
```math
x_{nuevo} = g(x)
```
Donde:
- _x_ es el valor actual.
- _g(x)_ es la función de iteración.

---

## Uso del Código

### 1. **Compilación**
Para compilar el código, asegúrate de tener instalados **Bison** y **Flex**. Luego, ejecuta los siguientes comandos en la terminal:

```bash
flex.exe –oparsermet.h parsermet.l
bison.exe -y -v --defines=parsermetTok.h -oparsermet_yac.h parsermet.y
```

Estos comandos generarán 3 archivos de cabecera. Si quieres generar un .h y un .c para poder realizar pruebas en consola necesitas ejecutar:

```bash
flex.exe –oparsermet.h parsermet.l
bison.exe -d –oparsermet_yac.c parsermet.y
```

### 2. **Archivo de Entrada**
Crea un archivo llamado `entrada.txt` en el mismo directorio que el ejecutable. El archivo debe contener la definición de la ecuación y los parámetros iniciales. Por ejemplo:

```
nt{
x = -2.0
f(x) = x^2 - 2
fd(x) = 2*x
error = 0.01
}
```

Esto indica que se utilizará el método de Newton-Raphson con un valor inicial de _x = -2.0_, la función:
```math
f(x) = x^2 - 2
```
, su derivada:

```math
f'(x) = 2*x
```

y un error esperado de 0.01.

### 3. **Ejecución**
Ejecuta el programa compilado si aplica.
Los resultados se imprimirán en un archivo llamado `resultados.txt`.

---

## Ejemplo de Salida

```
Iteración X            f(X)         g(X)         Error       
---------- ------------ ------------ ------------ ------------
1          0.400000     2.000000     0.400000     0.400000    
2          0.469404     0.347022     0.469404     0.069404    
3          0.498600     0.145980     0.498600     0.029196    
4          0.512893     0.071462     0.512893     0.014292    
5          0.520361     0.037343     0.520361     0.007469    
...
```

---

## Consideraciones

1. **División por Cero**: El código maneja la división por cero lanzando una advertencia.
2. **Precisión**: El algoritmo se detiene cuando el error es menor que el error esperado.
3. **Reinicio**: Después de cada ejecución, las variables globales se reinician a sus valores iniciales.

---

## Contribuciones

Si deseas contribuir a este proyecto, siéntete libre de hacer un fork y enviar un pull request con tus mejoras. Asegúrate de seguir las buenas prácticas de codificación y de documentar cualquier cambio importante.

---

## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.

---

¡Gracias por revisar este proyecto! Si tienes alguna pregunta o sugerencia, no dudes en abrir un issue en el repositorio.

## Creditos
- Joan Pablo Alvarado Garfias
- Jeremy Osorio Ramos
