# README

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

### 4. **Función Principal (`main`)**
- La función principal lee la entrada desde `entrada.txt`, realiza el análisis sintáctico, y luego ejecuta el método numérico seleccionado (Newton-Raphson o Punto Fijo) para encontrar la raíz de la ecuación.
- El proceso se repite hasta que el error sea menor que el error esperado.
- Los resultados de cada iteración se imprimen en la consola.

---

## Métodos Numéricos Implementados

### Método de Newton-Raphson
El método de Newton-Raphson es un algoritmo iterativo para encontrar la raíz de una función. Se basa en la siguiente fórmula:

> \[ x_{nuevo} = x - \frac{f(x)}{f'(x)} \]

Donde:
- _x_ es el valor actual.
- _f(x)_ es el valor de la función en _x_.
- _f'(x)_ es la derivada de la función en _x_.

### Método de Punto Fijo
El método de Punto Fijo es otro algoritmo iterativo que encuentra la raíz de una función transformando la ecuación original _f(x) = 0_ en una ecuación de la forma _x = g(x)_. La iteración se realiza mediante:

> \[ x_{nuevo} = g(x) \]

Donde:
- _x_ es el valor actual.
- _g(x)_ es la función de iteración.

---

## Uso del Código

### 1. **Compilación**
Para compilar el código, asegúrate de tener instalados **Bison** y **Flex**. Luego, ejecuta los siguientes comandos en la terminal:

```bash
bison -d newtonrap.y
flex newtonrap.l
gcc newtonrap.tab.c lex.yy.c -lm -o newtonrap
```

### 2. **Archivo de Entrada**
Crea un archivo llamado `entrada.txt` en el mismo directorio que el ejecutable. El archivo debe contener la definición de la ecuación y los parámetros iniciales. Por ejemplo:

```
NT_DEC 10 FX_DEC x^2 - 2 FDX_DEC 2*x ERROR_DEC 0.01 }
```

Esto indica que se utilizará el método de Newton-Raphson con un valor inicial de _x = 10_, la función _f(x) = x² - 2_, su derivada _f'(x) = 2x_, y un error esperado de 0.01.

### 3. **Ejecución**
Ejecuta el programa compilado:

```bash
./newtonrap
```

Los resultados se imprimirán en la consola y también se escribirán en el archivo `salida.txt`.

---

## Ejemplo de Salida

```
Iteracion 1:
  x = 5.100000
  Error = 4.900000
  f(x) = 24.010000
  f'(x) = 10.200000
Iteracion 2:
  x = 2.746078
  Error = 2.353922
  f(x) = 5.540947
  f'(x) = 5.492157
...
```

---

## Consideraciones

1. **División por Cero**: El código maneja la división por cero lanzando un error.
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
