import matplotlib.pyplot as plt

# Dati di esempio
categorie = ['Min-partition', 'Max-partition', 'Median-partition']
valori = [
1872556.05,
106740682.6,
31385968.44
]

# Creazione del grafico a barre
plt.bar(categorie, valori, color=['blue','orange','green'])

# Aggiunta di etichette agli assi e titolo al grafico
#plt.xlabel('Categorie')
#plt.ylabel('Valori')
plt.title('Partition quality in sequential mode')

# Mostra il grafico
plt.show()
