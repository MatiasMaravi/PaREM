import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Nuevos datos de los tiempos promedio en microsegundos
data = {
    'N': ['10k', '10k', '10k', '10k', '100k', '100k', '100k', '100k', '500k', '500k', '500k', '500k', '1M', '1M', '1M', '1M'],
    'Threads': [2, 4, 8, 16, 2, 4, 8, 16, 2, 4, 8, 16, 2, 4, 8, 16],
    'Average Time (microseconds)': [
        15592.3, 7290.47, 15250.8, 6487.05,
        18209.4, 15104.8, 15407.7, 11546.9,
        71659, 47311.6, 47666.2, 43137.6,
        130468, 75718.7, 87051, 82637.6
    ]
}

# Crear un DataFrame a partir de los nuevos datos
df = pd.DataFrame(data)

# Establecer el estilo de Seaborn
sns.set(style="whitegrid")

# Crear un gráfico de líneas
plt.figure(figsize=(10, 6))

# Usar seaborn para crear un gráfico de líneas, agrupado por el número de hilos
sns.lineplot(x="Threads", y="Average Time (microseconds)", hue="N", data=df, marker='o')

# Etiquetas y título del gráfico
plt.title("Tiempo promedio de ejecución en función del número de hilos y tamaño de los datos", fontsize=14)
plt.xlabel("Número de hilos", fontsize=12)
plt.ylabel("Tiempo promedio (microsegundos)", fontsize=12)
plt.legend(title="Tamaño de los datos (N)", title_fontsize='13', loc='upper right')

# Mostrar el gráfico
plt.show()