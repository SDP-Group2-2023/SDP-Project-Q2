import random, sys

def generateGraph(inputFileName : str, outputFileName : str) -> None:
    f_in = open(inputFileName, "r")
    f_out = open(outputFileName, "x")

    N = int(f_in.readline())
    f_out.write(f"{N}\n")
    for _ in range(N):
        val = f_in.readline()
        f_out.write(f"{val.strip()} {random.randint(1,100)}\n")
    f_in.close()
    f_out.close()

if __name__ == "__main__":
    if len(sys.argv[1:]) != 2:
        print("Usage: python3 graphGeneratorV2.py <input_file> <output_file>")
    else:
        generateGraph(sys.argv[1], sys.argv[2])