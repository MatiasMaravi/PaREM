# PaREM

## Autómatas
Nuestros autómatas están creados en la carpeta "automatas" y son los siguientes:
- B_word
- BANANA_word
- PARALLEL_word

## Optimización
Para poder calcular de manera automática los tiempo es necesario correr el siguiente comando:
```zsh
sh run.sh
```
Se presentará un pequeño menú con las opciones de ejecución, para poder correr el programa es necesario.
- Opción 1. Ejecutar el programa con el autómata "B_word".
- Opción 2. Ejecutar el programa con el autómata "BANANA_word".
- Opción 3. Ejecutar el programa con el autómata "PARALLEL_word".

Esto ejecutará automáticamente el algoritmo de manera secuencial y paralela con distintos tamaños de cadena y distintos números de hilos. Los gráficos de tiempo serán generados automáticamente en la carpeta "graficos_<nombre del autómata>"