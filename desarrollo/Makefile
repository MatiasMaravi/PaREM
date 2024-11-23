# Variables
GXX = mpic++
P = 4
TARGETS = version1 version2 version3

# Regla principal: compila todo
all: $(TARGETS)

# Regla genérica para compilar cada objetivo
%: %.cpp
	$(GXX) -o run-$@.out $<

# Reglas para ejecutar cada versión
run-%: %
	mpirun -np $(P) $@.out

# Limpieza de archivos generados
clean:
	rm -f *.out
