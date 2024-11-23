# Variables
GXX = mpic++
CFLAGS = -Iheaders
TARGETS = trivial_reduction_mpi binary_reduction_mpi
P = 4

# Regla principal (compila todo)
all: $(TARGETS)

# Reglas para cada ejecutable
%: %.cpp
	$(GXX) $(CFLAGS) -o $@.out $<

# Regla para ejecutar trivial_reduction_mpi
run-trivial: trivial_reduction_mpi
	mpirun -np $(P) trivial_reduction_mpi.out

# Regla para ejecutar binary_reduction_mpi
run-binary: binary_reduction_mpi
	mpirun -np $(P) binary_reduction_mpi.out

# Limpieza de archivos generados
clean:
	rm -f *.out
