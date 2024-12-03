#!/bin/bash

# Mostrar opciones al usuario
echo "Seleccione la directiva para compilar:"
echo "1) B_WORD"
echo "2) BANANA_WORD"
echo "3) PARALLEL_WORD"
read -p "Ingrese el número de su elección: " choice

# Asignar la directiva según la elección del usuario
case $choice in
    1)
        directive="B_WORD_"
        ;;
    2)
        directive="BANANA_WORD_"
        ;;
    3)
        directive="PARALLEL_WORD_"
        ;;
    *)
        echo "Opción no válida. Abortando."
        exit 1
        ;;
esac

# Compilar el programa con la directiva seleccionada
echo "Compilando con la directiva $directive..."
cd sequential/
make clean
make $directive

cd ..
cd omp/
make clean
make trivial_$directive
cd ..
python3 calcular_tiempo.py graficos_$directive

echo "Ejecución finalizada."