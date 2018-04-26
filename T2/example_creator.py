from random import randint

with open('input.txt', 'w') as arch:
    for i in range(256):
        arch.write("{}\n".format(str(randint(0, 268435455))))