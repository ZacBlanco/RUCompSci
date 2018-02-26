

files = ['results.txt', 'results2.txt']

def mean(l):
    return float(sum(l)) / max(len(l), 1)

def main():
    for filename in files:
        print(filename)
        runtimes = []
        cells = []
        with open(filename, 'r') as f:
            for line in f.readlines():
                if line.startswith('Finished running'):
                    runtimes.append(float(line.split(' ')[3]))
                elif line.startswith('Expanded'):
                    cells.append(int(line.split(' ')[1]))
        print(min(runtimes))
        print(min(cells))
        print(max(runtimes))
        print(max(cells))
        print(mean(runtimes))
        print(mean(cells))




if __name__ == "__main__":
    main()