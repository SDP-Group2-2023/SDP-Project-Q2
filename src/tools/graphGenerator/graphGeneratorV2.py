import random, sys

def generate_graph(input_file_name : str, output_file_name : str) -> None:
    f_in = open(input_file_name, "r")
    f_out = open(output_file_name, "x")

    flag = False
    while not flag:
        line = f_in.readline().split()
        if line[0] == "p":
            flag = True
            N = int(line[2])

    print(f"Generating graph with {N} nodes...")

    f_out.write(f"{N}\n")
    for i in range(N):
        f_out.write(f"{i+1} {random.randint(1,100)}\n")
    N = int(int(line[3])/2)

    print(f"Generating graph with {N} edges...")

    f_out.write(f"{N}\n")
    for _ in range(N):
        line = f_in.readline().split()
        f_in.readline()
        f_out.write(f"{line[1]} {line[2]} {line[3]}\n")
    f_in.close()
    f_out.close()

if __name__ == "__main__":
    if len(sys.argv[1:]) != 2:
        print("Usage: python3 (or python on windows) graphGeneratorV2.py <input_file> <output_file>")
    else:
        generate_graph(sys.argv[1], sys.argv[2])