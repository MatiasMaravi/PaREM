import os
import matplotlib.pyplot as plt
import pandas as pd
import re
import argparse
# Función para leer el archivo secuencial
def leer_tiempos_secuenciales(archivo):
    tiempos = {}
    with open(archivo, 'r') as f:
        for line in f:
            if line.startswith("N:"):
                n = re.search(r"N: (\w+)", line).group(1)
            elif line.startswith("Average time:"):
                tiempo = float(re.search(r"Average time: ([\d.]+)", line).group(1))
                tiempos[n] = tiempo
    return tiempos

# Función para leer el archivo paralelo
def leer_tiempos_paralelos(archivo):
    datos = []
    with open(archivo, 'r') as f:
        for line in f:
            if line.startswith("N:"):
                n_threads_match = re.search(r"N: (\w+) Threads: (\d+)", line)
                n = n_threads_match.group(1)
                threads = int(n_threads_match.group(2))
            elif line.startswith("Average time:"):
                tiempo = float(re.search(r"Average time: ([\d.]+)", line).group(1))
                datos.append((n, threads, tiempo))
    return pd.DataFrame(datos, columns=["N", "Threads", "Time"])

# Crear la carpeta "graficos"
# Configuración de argparse
parser = argparse.ArgumentParser(description="Generar gráficos de tiempos secuenciales y paralelos")
parser.add_argument("output_dir", type=str, help="Nombre de la carpeta para guardar los gráficos")
args = parser.parse_args()
output_dir = args.output_dir
os.makedirs(output_dir, exist_ok=True)

# Leer los datos
tiempos_secuenciales = leer_tiempos_secuenciales("sequential/results_sequential.txt")
tiempos_paralelos = leer_tiempos_paralelos("omp/results_trivial_omp.txt")

# Crear gráficos individuales para cada valor de N
for n in tiempos_paralelos["N"].unique():
    subset = tiempos_paralelos[tiempos_paralelos["N"] == n]
    secuencial_time = tiempos_secuenciales.get(n, None)
    
    plt.figure(figsize=(8, 6))
    
    # Graficar tiempo secuencial como línea horizontal
    if secuencial_time is not None:
        plt.hlines(y=secuencial_time, xmin=subset["Threads"].min(), xmax=subset["Threads"].max(),
                   colors='gray', linestyles='dashed', label=f"Secuencial ({n})")
    
    # Graficar tiempos paralelos
    plt.plot(subset["Threads"], subset["Time"], marker='o', label=f"Paralelo ({n})")
    
    # Configuración del gráfico
    plt.xscale("log", base=2)  # Eje X en escala logarítmica
    plt.xlabel("Cantidad de Threads", fontsize=12)
    plt.ylabel("Tiempo Promedio (microseconds)", fontsize=12)
    plt.title(f"Comparación de Tiempos para N = {n}", fontsize=14)
    plt.legend()
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    
    # Guardar el gráfico
    output_path = os.path.join(output_dir, f"grafico_{n}.png")
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()

print(f"Gráficos generados y guardados en la carpeta '{output_dir}'.")

