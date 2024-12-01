# Variables
GXX = g++
CFLAGS = -Iheaders -fopenmp
TARGETS = main

# Regla principal (compila todo)
all: $(TARGETS)

# Regla para compilar cualquier ejecutable
%: %.cpp
	$(GXX) $(CFLAGS) -o $@ $<

# Regla para ejecutar trivial_reduction_omp
run: main
	./main

# Limpieza de archivos generados
clean:
	rm -f $(TARGETS)
