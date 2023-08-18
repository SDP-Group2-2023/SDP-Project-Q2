import networkx as nx
import matplotlib.pyplot as plt

class GraphVisualization:
    
        def __init__(self):
            self.visual = []
    
        def addEdge(self, a, b):
            temp = [a, b]
            self.visual.append(temp)
    
        def visualize(self):
            G = nx.Graph()
            G.add_edges_from(self.visual)
            nx.draw_networkx(G)
            plt.show()

G = GraphVisualization()
file_path = input("Inserisci il path del file: ")

with open(file_path, "rb") as file:
    # Sposta il cursore di lettura alla posizione desiderata (ad esempio, al byte 100)
    
    # Ora puoi leggere i dati dalla posizione specificata
    binary_data = file.read(4)  # Leggi 4 byte dalla posizione corrente
    num_nodes = int.from_bytes(binary_data, byteorder='little', signed=False)
    print(num_nodes)

    binary_data = file.read(4)  # Leggi 4 byte dalla posizione corrente
    num_edges = int.from_bytes(binary_data, byteorder='little', signed=False)
    print(num_edges)
    
    file.seek(8 + num_nodes * 8)
    for i in range(num_edges):
        binary_data = file.read(12)
        node1 = int.from_bytes(binary_data[0:4], byteorder='little', signed=False)
        node2 = int.from_bytes(binary_data[4:8], byteorder='little', signed=False)
        weight = int.from_bytes(binary_data[8:12], byteorder='little', signed=False)
        #print(node1, node2, weight)
        G.addEdge(node1, node2)

print("Visualizzazione grafo...")
G.visualize()