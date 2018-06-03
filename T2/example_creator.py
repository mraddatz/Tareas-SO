from random import randint

with open('input.txt', 'w') as arch:
    for i in range(2560):
        arch.write("{}\n".format(str(randint(268400000, 268435455))))