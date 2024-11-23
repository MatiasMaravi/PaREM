# Variables
GXX = g++
CFLAGS = -Iheaders -fopenmp
TARGETS = trivial_reduction_omp binary_reduction_omp

# Regla principal (compila todo)
all: $(TARGETS)

# Regla para compilar cualquier ejecutable
%: %.cpp
	$(GXX) $(CFLAGS) -o $@ $<

# Regla para ejecutar trivial_reduction_omp
run-trivial: trivial_reduction_omp
	./trivial_reduction_omp

# Regla para ejecutar binary_reduction_omp
run-binary: binary_reduction_omp
	./binary_reduction_omp

# Limpieza de archivos generados
clean:
	rm -f $(TARGETS)
