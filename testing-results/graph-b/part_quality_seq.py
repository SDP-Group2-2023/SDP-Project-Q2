import matplotlib.pyplot as plt

# Dati di esempio
categorie = ['Min-partition', 'Max-partition', 'Avg-partition', 'Median-partition']
valori = [
461444,
14871666,
5303652,
5443421.08
]

# Creazione del grafico a barre
plt.bar(categorie, valori, color='blue')

# Aggiunta di etichette agli assi e titolo al grafico
#plt.xlabel('Categorie')
#plt.ylabel('Valori')
plt.title('Partition quality in sequential mode')

# Mostra il grafico
plt.show()
