import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [
19128.23,
15746.72,
19125.82,
14950.32,
16349.89,
16646.2,
16798.13,
16921.21


]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('partitioning time in parallel mode')

# Mostra il grafico
plt.show()
