import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [1089.83,1004.21,989.71,974.32,981.06,984.08,990.91,996.68]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('loading time')

# Mostra il grafico
plt.show()
