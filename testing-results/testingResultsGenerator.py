import matplotlib.pyplot as plt
with open('C:\\Users\\edoar\\code\\sdp-project\\testing-results\\graph-c\\c_testing.txt', 'r') as file:
    # Leggere tutte le linee del file in una lista
    lines = file.readlines()
    # LOADING TIME
    x = []
    for i in lines[0].strip().split(','):
        x.append(int(i))

    y = []
    for i in lines[1].strip().split(','):
        y.append(float(i))
  
    # Creazione del grafico con linea che unisce i punti
    plt.plot(x, y, marker='o', linestyle='-')

    # Aggiunta di etichette agli assi e titolo al grafico
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('loading time')

    # Mostra il grafico
    plt.show()

    # PARALLEL PARTITIONING EXECUTION TIME
    x = []
    for i in lines[2].strip().split(','):
        x.append(int(i))

    y = []
    for i in lines[3].strip().split(','):
        y.append(float(i))

    # Creazione del grafico con linea che unisce i punti
    plt.plot(x, y, marker='o', linestyle='-')

    # Aggiunta di etichette agli assi e titolo al grafico
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('partitioning time in parallel mode')

    # Mostra il grafico
    plt.show()

    # SEQUENTIAL PARTITION QUALITY
    categorie = ['Min-partition', 'Max-partition', 'Median-partition']
    valori = []
    for i in lines[4].strip().split(','):
            valori.append(float(i))

    # Creazione del grafico a barre
    plt.bar(categorie, valori, color=['blue','orange','green'])

    # Aggiunta di etichette agli assi e titolo al grafico
    #plt.xlabel('Categorie')
    #plt.ylabel('Valori')
    plt.title('Partition quality in sequential mode')

    # Mostra il grafico
    plt.show()


    # PARALLEL PARTITION QUALITY
    x1 = []
    for i in lines[5].strip().split(','):
        x1.append(int(i))

    y1 = []
    for i in lines[6].strip().split(','):
        y1.append(float(i))

    # Dati di esempio per la seconda linea
    x2 = []
    for i in lines[7].strip().split(','):
        x2.append(int(i))

    y2 = []
    for i in lines[8].strip().split(','):
        y2.append(float(i))

    # Dati di esempio per la quart linea
    x3 = []
    for i in lines[9].strip().split(','):
        x3.append(int(i))

    y3 = []
    for i in lines[10].strip().split(','):
        y3.append(float(i))

    # Creazione del grafico con due linee
    plt.plot(x1, y1, label='Min-partition', marker='o', linestyle='-')
    plt.plot(x2, y2, label='Max-partition', marker='s', linestyle='--')
    plt.plot(x3, y3, label='Median-partition', marker='D', linestyle=':')

    # Aggiunta di etichette agli assi, titolo al grafico e legenda
    plt.xlabel('threads')
    #plt.ylabel('Asse Y')
    plt.title('Partition quality in parallel mode')
    #plt.legend(loc="best")

    # Mostra il grafico
    plt.show()
