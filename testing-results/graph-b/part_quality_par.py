import matplotlib.pyplot as plt

# Dati di esempio per la prima linea
x1 = [1, 2, 3, 4, 5, 6, 7, 8]
y1 = [
2775705.91,
2704344.47,
2677255.14,
2667081.33,
2587723.45,
2559690.35,
2611360.69,
2523155.86

]

# Dati di esempio per la seconda linea
x2 = [1, 2, 3, 4, 5, 6, 7, 8]
y2 = [
8668476.69,
8657587.12,
8708545.35,
8642455.39,
8583318.91,
8717600.62,
8639209.51,
8847077.08

]

# Dati di esempio per la terza linea
x3 = [1, 2, 3, 4, 5, 6, 7, 8]
y3 = [
5297402.8,
5294613.26,
5298806.37,
5296580.27,
5299818.12,
5300534.07,
5299354.48,
5301692.22

]

# Dati di esempio per la quart linea
x4 = [1, 2, 3, 4, 5, 6, 7, 8]
y4 = [
5309063.41,
5306732.41,
5320309.59,
5309609.52,
5309216.74,
5314515.05,
5325824.23,
5329791.98

]

# Creazione del grafico con due linee
plt.plot(x1, y1, label='Min-partition', marker='o', linestyle='-')
plt.plot(x2, y2, label='Max-partition', marker='s', linestyle='--')
plt.plot(x3, y3, label='Avg-partition', marker='^', linestyle='-.')
plt.plot(x4, y4, label='Median-partition', marker='D', linestyle=':')

# Aggiunta di etichette agli assi, titolo al grafico e legenda
plt.xlabel('threads')
#plt.ylabel('Asse Y')
plt.title('Partition quality in parallel mode')
#plt.legend(loc="best")

# Mostra il grafico
plt.show()
