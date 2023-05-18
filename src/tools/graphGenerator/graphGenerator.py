import random, sys

def main(argv):
    if len(argv) != 2:
        print("Usage: python graphGenerator.py <number of vertices> <name of the file>")
        return
    N = int(argv[0])
    name = argv[1]
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

main(sys.argv[1:])