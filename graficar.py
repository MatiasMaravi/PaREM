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
    
    plt.figure(figsize=(10, 7))

    for n in tiempos_paralelos["N"].unique():
        subset = tiempos_paralelos[tiempos_paralelos["N"] == n]
        secuencial_time = tiempos_secuenciales.get(n, None)
        if(n == "10k"):
            n_valor = 10000
        elif(n == "100k"):
            n_valor = 100000
        elif(n == "500k"):
            n_valor = 500000
        elif(n == "1M"):
            n_valor = 1000000
        
        if secuencial_time is not None:
            # Calcular y graficar speedup real
            speedup_real = secuencial_time / subset["Time"]
            plt.plot(subset["Threads"], speedup_real, marker='o', label=f"Speedup Real (N={n})")
            
            # Calcular y graficar speedup teórico
            speedup_teorico = [n_valor / ((n_valor / p) * 7) for p in subset["Threads"]]
            plt.plot(subset["Threads"], speedup_teorico, linestyle='--', marker='x', label=f"Speedup Teórico (N={n})")

    # Configuración del gráfico
    plt.xscale("log", base=2)
    plt.xlabel("Cantidad de Threads", fontsize=12)
    plt.ylabel("Speedup", fontsize=12)
    plt.title("Speedup: Real vs Teórico", fontsize=14)
    plt.legend(title="Valores de N", fontsize=10)
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)

    # Guardar el gráfico
    speedup_path = os.path.join(output_dir, "speedup_grafico.png")
    plt.tight_layout()
    plt.savefig(speedup_path)
    plt.close()


print(f"Gráficos generados y guardados en la carpeta '{output_dir}'.")
