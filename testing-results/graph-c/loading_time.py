import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [
6143.03,
5617.35,
5534.2,
5448.51,
5475.76,
5488.69,
5546.7,
5583.06
]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('loading time')

# Mostra il grafico
plt.show()
