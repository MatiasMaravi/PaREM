# Variables
GXX = g++
CFLAGS = -Iheaders -fopenmp
TARGET = main
SRC = main.cpp

# Objetivos "phony"
.PHONY: all B_WORD_ BANANA_WORD_ PARALLEL_WORD_ clean

# Reglas principales
all: B_WORD_ BANANA_WORD_ PARALLEL_WORD_

# Reglas para compilar y ejecutar cada directiva
B_WORD_: $(SRC)
	$(GXX) $(CFLAGS) -DB_WORD -o $(TARGET)_bword $<
	@echo "Ejecutando $(TARGET)_bword..."
	./$(TARGET)_bword

BANANA_WORD_: $(SRC)
	$(GXX) $(CFLAGS) -DBANANA_WORD -o $(TARGET)_banana $<
	@echo "Ejecutando $(TARGET)_banana..."
	./$(TARGET)_banana

PARALLEL_WORD_: $(SRC)
	$(GXX) $(CFLAGS) -DPARALLEL_WORD -o $(TARGET)_parallel $<
	@echo "Ejecutando $(TARGET)_parallel..."
	./$(TARGET)_parallel

# Limpieza de archivos generados
clean:
	rm -f $(TARGET)_bword $(TARGET)_banana $(TARGET)_parallel
