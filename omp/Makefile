# Variables
GXX = g++
CFLAGS = -Iheaders -fopenmp
TARGETS = trivial_reduction_omp

# Objetivos "phony"
.PHONY: all clean trivial_BANANA_ trivial_B_WORD_ trivial_PARALLEL_

# Reglas principales
all: trivial_BANANA_ binary_BANANA_ trivial_B_WORD_ binary_B_WORD_ trivial_PARALLEL_ binary_PARALLEL_

# Reglas para compilar y ejecutar con BANANA directiva
trivial_BANANA_WORD_: trivial_reduction_omp.cpp
	$(GXX) $(CFLAGS) -DBANANA_WORD -o trivial_reduction_omp_banana trivial_reduction_omp.cpp
	@echo "Ejecutando trivial_reduction_omp_banana..."
	./trivial_reduction_omp_banana

# Reglas para compilar y ejecutar con B_WORD directiva
trivial_B_WORD_: trivial_reduction_omp.cpp
	$(GXX) $(CFLAGS) -DB_WORD -o trivial_reduction_omp_bword trivial_reduction_omp.cpp
	@echo "Ejecutando trivial_reduction_omp_bword..."
	./trivial_reduction_omp_bword

# Reglas para compilar y ejecutar con PARALLEL_WORD directiva
trivial_PARALLEL_WORD_: trivial_reduction_omp.cpp
	$(GXX) $(CFLAGS) -DPARALLEL_WORD -o trivial_reduction_omp_parallel trivial_reduction_omp.cpp
	@echo "Ejecutando trivial_reduction_omp_parallel..."
	./trivial_reduction_omp_parallel

# Limpieza de archivos generados
clean:
	rm -f trivial_reduction_omp_* 
