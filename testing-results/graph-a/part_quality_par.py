import matplotlib.pyplot as plt

# Dati di esempio per la prima linea
x1 = [1, 2, 3, 4, 5, 6, 7, 8]
y1 = [
902146.63,
899090.66,
889415.86,
898119.71,
869193.77,
865723.87,
864398.48,
852866.2
]

# Dati di esempio per la seconda linea
x2 = [1, 2, 3, 4, 5, 6, 7, 8]
y2 = [
1797285.92,
1812703.8,
1820932.93,
1807312.62,
1808757.03,
1821892.12,
1841008.26,
1840910.75
]

# Dati di esempio per la quart linea
x4 = [1, 2, 3, 4, 5, 6, 7, 8]
y4 = [
1321449.5,
1320169.97,
1320770.45,
1318184.42,
1322626.5,
1323832.3,
1321689.23,
1321169.14
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
