# Variables
GXX = g++
CFLAGS = -Iheaders -fopenmp
TARGETS = version1 version2 version3

# Regla principal (compila todo)
all: $(TARGETS)

# Regla para compilar cualquier ejecutable
%: %.cpp
	$(GXX) $(CFLAGS) -o $@ $<

# Regla para ejecutar trivial_reduction_omp
run-version1: version1
	./version1

run-version2: version2
	./version2

run-version3: version3
	./version3


# Limpieza de archivos generados
clean:
	rm -f $(TARGETS)
