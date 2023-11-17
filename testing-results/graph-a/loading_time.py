import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [284.84,264.21,261.23,257.53,260.45,261.2,261.94,262.34]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('loading time')

# Mostra il grafico
plt.show()
