import matplotlib.pyplot as plt

# Dati di esempio
categorie = ['Min-partition', 'Max-partition', 'Median-partition']
valori = [
163856.88,
3947381.96,
1323154.32
]

# Creazione del grafico a barre
plt.bar(categorie, valori, color='blue')

# Aggiunta di etichette agli assi e titolo al grafico
#plt.xlabel('Categorie')
#plt.ylabel('Valori')
plt.title('Partition quality in sequential mode')

# Mostra il grafico
plt.show()
