import matplotlib.pyplot as plt

# Dati di esempio per la prima linea
x1 = [1, 2, 3, 4, 5, 6, 7, 8]
y1 = [
17859797.14,
18003290.03,
17441895.13,
17531174.32,
17968922.44,
17684779.41,
17797584.07,
17481150.95
]

# Dati di esempio per la seconda linea
x2 = [1, 2, 3, 4, 5, 6, 7, 8]
y2 = [
47376775.96,
47123829.84,
47336560.63,
46986528.04,
47421576.39,
47635805.67,
46877349.01,
47450939.36
]


# Dati di esempio per la quart linea
x4 = [1, 2, 3, 4, 5, 6, 7, 8]
y4 = [
31177428,
31179427.54,
31297164.38,
31334764.29,
31164998.76,
31180761.74,
31290410.06,
31217354.86
]

# Creazione del grafico con due linee
plt.plot(x1, y1, label='Min-partition', marker='o', linestyle='-')
plt.plot(x2, y2, label='Max-partition', marker='s', linestyle='--')
plt.plot(x4, y4, label='Median-partition', marker='D', linestyle=':')

# Aggiunta di etichette agli assi, titolo al grafico e legenda
plt.xlabel('threads')
#plt.ylabel('Asse Y')
plt.title('Partition quality in parallel mode')
#plt.legend(loc="best")

# Mostra il grafico
plt.show()
