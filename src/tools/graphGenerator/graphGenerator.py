import random, sys

def generateGraph(vertices : int, fileName : str) -> None:
    N = int(vertices)
    name = str(fileName)
    f = open(f"{name}.txt", "x")
    f.write(f"{N}\n")
    for i in range(N):
        f.write(f"{i} {random.randint(0, 100)}\n")

    for i in range(N):
        for j in range(i,N):
            if i!=j and random.randint(0, 100) == 10: # 1% chance of having an edge
                val = random.randint(1, 100)
                f.write(f"{i} {j} {val}\n")
                print(f"{i} {j} {val}")
    f.close()

if __name__ == "__main__":
    if len(sys.argv[1:]) != 2:
        print("Usage: python3 graphGenerator.py <number of vertices> <name of the file>")
    else:
        generateGraph(sys.argv[1], sys.argv[2])