import matplotlib.pyplot as plt
import numpy as np

# =======================================================
# GRÁFICO 1: ESCALABILIDAD (Tiempo vs Cantidad de Datos)
# Comparando la explosión O(n^2) de FFDH vs el O(n) de NFDH
# =======================================================

# 1. Reemplaza estos valores con tus tiempos reales en milisegundos (ms)
n_valores = ['10', '10^3', '10^6']
tiempos_NFDH = [0.1, 4.5, 1250.0]   # Tiempos de tu heurística optimizada
tiempos_FFDH = [0.2, 45.0, 315000.0] # Tiempos aproximados del algoritmo sin optimizar

plt.figure(figsize=(10, 6))
plt.plot(n_valores, tiempos_NFDH, marker='o', linestyle='-', color='b', linewidth=2, label='NFDH O(n)')
plt.plot(n_valores, tiempos_FFDH, marker='s', linestyle='--', color='r', linewidth=2, label='FFDH O(n^2)')

plt.title('Comparación de Escalabilidad Algorítmica', fontsize=14, fontweight='bold')
plt.xlabel('Cantidad de Piezas (n)', fontsize=12)
plt.ylabel('Tiempo de Ejecución (milisegundos)', fontsize=12)
plt.grid(True, linestyle=':', alpha=0.7)
plt.legend(fontsize=12)

# Guardar la imagen automáticamente
plt.savefig('grafico_escalabilidad.png', dpi=300, bbox_inches='tight')
print("✅ Gráfico de escalabilidad generado: grafico_escalabilidad.png")


# =======================================================
# GRÁFICO 2: NATURALEZA DE LOS DATOS
# Impacto del ordenamiento en la heurística para N = 10^6
# =======================================================

# 2. Reemplaza estos valores con los tiempos que te dio la opción 3 de tu menú
naturaleza = ['Aleatorio\n(Caso Promedio)', 'Ordenado Ascendente\n(Peor Caso)', 'Ordenado Descendente\n(Mejor Caso)']
tiempos_naturaleza = [1250.0, 1400.0, 950.0] 

plt.figure(figsize=(9, 6))
barras = plt.bar(naturaleza, tiempos_naturaleza, color=['#3498db', '#e74c3c', '#2ecc71'], edgecolor='black')

plt.title('Impacto de la Naturaleza de los Datos (N = 10^6)', fontsize=14, fontweight='bold')
plt.ylabel('Tiempo de Ejecución (milisegundos)', fontsize=12)
plt.grid(axis='y', linestyle=':', alpha=0.7)

# Agregar los números exactos encima de cada barra
for barra in barras:
    yval = barra.get_height()
    plt.text(barra.get_x() + barra.get_width()/2, yval + 15, f'{yval} ms', ha='center', va='bottom', fontweight='bold')

# Guardar la imagen automáticamente
plt.savefig('grafico_naturaleza.png', dpi=300, bbox_inches='tight')
print("✅ Gráfico de naturaleza de datos generado: grafico_naturaleza.png")

plt.show() # Mostrar los gráficos en pantalla