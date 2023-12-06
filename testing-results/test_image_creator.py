import csv
import sys
import matplotlib.pyplot as plt

def plot_read(read_res_file, threads, iterations):

    x = []
    y = []
    dizionario = {}
    for i in range(1, threads+1):
        dizionario[i] = 0.0


    with open(read_res_file) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                line_count += 1
            else:
                dizionario[int(row[0])] += float(row[1])
                line_count += 1

    for key in dizionario:
        x.append(key)
        y.append(dizionario[key]/iterations)

    # Creazione del grafico con linea che unisce i punti
    plt.plot(x, y, marker='o', linestyle='-')

    # Aggiunta di etichette agli assi e titolo al grafico
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('loading time')

    # Mostra il grafico
    plt.savefig(read_res_file + '.png')
    plt.clf()

def plot_seq(seq_res_file, iterations):
 # SEQUENTIAL PARTITION QUALITY
    categorie = ['Min-partition', 'Max-partition', 'Median-partition']
    valori = {}
    allCategories = []
    
    with open(seq_res_file) as csv_file:
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

    valori_to_print = [valori['min_partition']/iterations, valori['max_partition']/iterations, valori['median_partition']/iterations]

    print(categorie)
    print(valori_to_print)

    # Creazione del grafico a barre
    plt.bar(categorie, valori_to_print, color=['blue','orange','green'])

    # Aggiunta di etichette agli assi e titolo al grafico
    #plt.xlabel('Categorie')
    #plt.ylabel('Valori')
    plt.title('Partition quality in sequential mode')

    # Mostra il grafico
    plt.savefig("seq_quality_" + graph_name + '.png')
    plt.clf()


def plot_par(path, threads, iterations):

    allCategories = []
    valori = []
    for i in range(threads):
        valori.append({})

    with open(path) as csv_file:
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
                for i in range(1, len(row)-1):
                    valori[int(row[0])-1][allCategories[i]] += float(row[i])
                line_count += 1

    

    for i in range(threads):
        for key in valori[i]:
            valori[i][key] = valori[i][key]/iterations

    runTime = []
    for i in range( threads):
        runTime.append(valori[i][allCategories[1]])

    plt.plot(range(1, threads+1), runTime, marker='o', linestyle='-')
    plt.xlabel('threads')
    plt.ylabel('time (ms)')
    plt.title('partitioning time in parallel mode')
    plt.savefig("par_time_" + graph_name + '.png')
    plt.clf()

    min_partition = []
    for i in range( threads):
        min_partition.append(valori[i][allCategories[3]])
    max_partition = []
    for i in range( threads):
        max_partition.append(valori[i][allCategories[4]])
    median_partition = []
    for i in range( threads):
        median_partition.append(valori[i][allCategories[6]])
    # Creazione del grafico con due linee
    plt.plot(range(1, threads+1), min_partition, label='Min-partition', marker='o', linestyle='-')
    plt.plot(range(1, threads+1), max_partition, label='Max-partition', marker='s', linestyle='--')
    plt.plot(range(1, threads+1), median_partition, label='Median-partition', marker='D', linestyle=':')

    print("PARALELL: ",valori[3])

    # Aggiunta di etichette agli assi, titolo al grafico e legenda
    plt.xlabel('threads')
    #plt.ylabel('Asse Y')
    plt.title('Partition quality in parallel mode')
    #plt.legend(loc="best")

    # Mostra il grafico
    plt.savefig("par_quality_" + graph_name + '.png')

def plot_graphs(graph_name, threads, iterations):
    plot_read("read_test_" + graph_name +".csv", threads, iterations)
    plot_seq("seq_test_"+graph_name+".csv", iterations)
    plot_par("par_test_"+graph_name+".csv", threads, iterations)


if __name__ == '__main__':

    graph_name = sys.argv[1]
    threads = int(sys.argv[2])
    iterations = int(sys.argv[3])
    plot_graphs(graph_name.lower(), threads, iterations)