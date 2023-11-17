import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [
4572.45,
3636.94,
4396.24,
3460.16,
3905.72,
3942.67,
3980.72,
4100.99

]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('partitioning time in parallel mode')

# Mostra il grafico
plt.show()
