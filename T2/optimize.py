N_BITS = 20
min_mem = 0
niveles = [0,0,0,0,0]
minimo = []

def optimize(n, bits, mem_sum=0):
    global min_mem
    global minimo
    if n == 1:
        if bits == N_BITS:
            tot_mem = mem_sum + ((2**bits) * 11)
        else:
            tot_mem = mem_sum + ((2**bits) * niveles[1])
        if tot_mem < min_mem or min_mem == 0:
            min_mem = tot_mem
            niveles[n-1] = bits
            minimo = [i for i in niveles]
        return
    for x in range(1, bits):
        niveles[n-1] = x
        if bits == N_BITS:
            x_mem = (2**x) * (11)
        else:
            x_mem = (2**x) * niveles[n]
        optimize(n-1, bits-x, mem_sum + x_mem)


for j in range(1,6):
    optimize(j, N_BITS)
    string = "Reparticion: "
    for i in minimo:
        string += "{}-".format(i)
    string += "bits\nMemoria usada {} bits".format(min_mem)
    print(string)
