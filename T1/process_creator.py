from random import randint
ARCH = "archivo"

def main():
    with open(ARCH, 'w') as arch:
        for i in range(50):
            nombre = "process" + str(i+1)
            T = randint(0, 15)
            N = randint(1, 10)
            bursts = ""
            for j in range(N):
                bursts += str(randint(1, 10)) + " "
            arch.write("{} {} {} {}".format(nombre, str(T), str(N), bursts[:-1]))
            if i < 49:
                arch.write("\n")

if __name__ == "__main__":
    main()

