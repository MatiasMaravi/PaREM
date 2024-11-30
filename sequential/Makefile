# Variables
GXX = g++
CFLAGS = -Iheaders
TARGETS = main

# Objetivos "phony"
.PHONY: all run clean

# Regla principal (compila todo)
all: $(TARGETS)

# Reglas para cada ejecutable
%: %.cpp
	$(GXX) $(CFLAGS) -o $@ $<

# Regla para ejecutar main
run: main
	./main

# Limpieza de archivos generados
clean:
	rm -f $(TARGETS)
