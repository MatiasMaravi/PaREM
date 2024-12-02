import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Nuevos datos de los tiempos promedio en microsegundos
data = {
    'N': ['10k', '10k', '10k', '10k', '100k', '100k', '100k', '100k', '500k', '500k', '500k', '500k', '1M', '1M', '1M', '1M'],
    'Threads': [2, 4, 8, 16, 2, 4, 8, 16, 2, 4, 8, 16, 2, 4, 8, 16],
    'Average Time (microseconds)': [
        10489.2, 8877, 15525.8, 6596.34,
        79430.4, 233886, 73383.4, 57422.4,
        377221, 337881, 340983, 299636,
        863546, 750601, 666588, 633074
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
