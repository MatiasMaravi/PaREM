# PaREM

## Optimización
Para poder calcular de manera automática los tiempo es necesario correr el siguiente comando (no funciona en MacOS):
```zsh
cd omp/
make run-trivial
```
Por defecto las pruebas se hacen con un autómata que acepta la cadena "parallel" y los textos de prueba están en la carpeta textos.
Si se desea utilizar otro autómata es necesario cambiarlo en la línea 2 del archivo trivial_reduction_omp.cpp
Para esto es necesario haber definido un autómata en la carpeta automatas y haberlo incluido en el archivo cpp.
```cpp
#include "../automatas/parallel_word.h"
```
Y en la línea 79 del mismo archivo. Esto es necesario porque tiene que leer los textos con los que se va a trabajar.
```cpp
string T = get_text("../textos/parallel_" + texto + ".txt",num_threads);
```
