import matplotlib.pyplot as plt

# Dati di esempio
x = [1, 2, 3, 4, 5, 6, 7 ,8]
y = [
139063.71,
115897.55,
138143.47,
111878.81,
118366.91,
119743.65,
123317.17,
120445.5



]

# Creazione del grafico con linea che unisce i punti
plt.plot(x, y, marker='o', linestyle='-')

# Aggiunta di etichette agli assi e titolo al grafico
plt.xlabel('threads')
plt.ylabel('time (ms)')
plt.title('partitioning time in parallel mode')

# Mostra il grafico
plt.show()
