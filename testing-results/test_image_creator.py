import csv
import sys
import matplotlib.pyplot as plt


def plot_read(graph_name, threads, iterations):
    x = []
    y = []
    i = 1
    dizionario = {}
    while i <= threads:
        dizionario[i] = 0.0

    with open("read_test_" + graph_name + ".csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count != 0:
                dizionario[int(row[0])] += float(row[1])
                line_count += 1

    for key in dizionario:
        x.append(key)
        y.append(dizionario[key] / iterations)

    # Creazione del grafico con linea che unisce i punti
    plt.plot(x, y, marker='o', linestyle='-')

    # Aggiunta di etichette agli assi e titolo al grafico
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('loading time')

    # Mostra il grafico
    plt.savefig("seq_test_"+ graph_name + '.png')
    plt.clf()


def plot_seq(graph_name, iterations):
    # SEQUENTIAL PARTITION QUALITY
    categorie = ['Min-partition', 'Max-partition', 'Median-partition']
    valori = {}
    allCategories = []

    with open("seq_test_" + graph_name + ".csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0

        for row in csv_reader:
            if line_count == 0:
                for a in row:
                    valori[a] = 0.0
                    allCategories.append(a)
                line_count += 1
            else:
                for i in range(0, len(row)):
                    valori[allCategories[i]] += float(row[i])
                line_count += 1

    for key in valori:
        valori[key] = valori[key] / iterations

    valori_to_print = [valori['min_partition'], valori['max_partition'], valori['median_partition']]

    print(allCategories)
    print(valori)

    # Creazione del grafico a barre
    plt.bar(categorie, valori_to_print, color=['blue', 'orange', 'green'])

    # Aggiunta di etichette agli assi e titolo al grafico
    # plt.xlabel('Categorie')
    # plt.ylabel('Valori')
    plt.title('Partition quality in sequential mode')

    # Mostra il grafico
    plt.savefig("seq_quality_" + graph_name + '.png')
    plt.clf()


def plot_par(graph_name, threads, iterations):
    allCategories = []
    valori = []
    for i in range(threads):
        valori.append({})

    with open("par_test_" + graph_name + ".csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                for a in row:
                    allCategories.append(a)
                    for t in range(0, threads):
                        valori[t][a] = 0.0

                line_count += 1
            else:
                for i in range(1, len(row)):
                    valori[int(row[0]) - 1][allCategories[i]] += float(row[i])
                line_count += 1

    for i in range(threads):
        for key in valori[i]:
            valori[i][key] = valori[i][key] / iterations

    runTime = []
    for i in range(threads):
        runTime.append(valori[i][allCategories[1]])

    plt.plot(range(1, threads + 1), runTime, marker='o', linestyle='-')
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('partitioning time in parallel mode')
    plt.savefig("par_time_" + graph_name + '.png')
    plt.clf()

    min_partition = []
    for i in range(threads):
        min_partition.append(valori[i][allCategories[3]])
    max_partition = []
    for i in range(threads):
        max_partition.append(valori[i][allCategories[4]])
    median_partition = []
    for i in range(threads):
        median_partition.append(valori[i][allCategories[6]])
    # Creazione del grafico con due linee
    plt.plot(range(1, threads + 1), min_partition, label='Min-partition', marker='o', linestyle='-')
    plt.plot(range(1, threads + 1), max_partition, label='Max-partition', marker='s', linestyle='--')
    plt.plot(range(1, threads + 1), median_partition, label='Median-partition', marker='D', linestyle=':')

    print("PARALELL: ", valori[3])

    # Aggiunta di etichette agli assi, titolo al grafico e legenda
    plt.xlabel('threads')
    # plt.ylabel('Asse Y')
    plt.title('Partition quality in parallel mode')
    # plt.legend(loc="best")

    # Mostra il grafico
    plt.savefig("par_quality_" + graph_name + '.png')


def plot_graphs(graph_name, threads, iterations):
    plot_read(graph_name, threads, iterations)
    plot_seq(graph_name, iterations)
    plot_par(graph_name, threads, iterations)


if __name__ == '__main__':
    plot_graphs(sys.argv[1].lower(), int(sys.argv[2]), int(sys.argv[3]))
