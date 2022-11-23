import subprocess
import os
import matplotlib.pyplot as plt
from math import log2

FNULL = open(os.devnull, 'w')

# block_sizes = [80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480, 40960, 81920, 16384, 32768]
# block_sizes = [80, 160, 320, 1280, 2560, 10240, 40960, 163840, 327680]
block_sizes = [960]
# block_sizes = [80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480]
# number_of_records = [10, 20, 40, 80, 160, 320, 640, 1280, 2560]
number_of_records = [10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480]

if __name__ == '__main__':
    # f = open("output.txt", "w")
    # f.close()
    # for i in block_sizes:
    #     for j in number_of_records:
    #         # if i < 320:
    #         #     if j > 2560:
    #         #         continue
    #         # if i < 2560:
    #         #     if j > 10240:
    #         #         continue
    #         args = "bd1.exe -g -s -p -o output.txt -b " + str(i) + " -n " + str(j)
    #         print(args)
    #         subprocess.call(args)

    wykres_disk_accesses = [[], []]
    wykres_phases = [[], []]

    with open('output.txt') as f:
        for line in f:
            a = [int(x) for x in line.split()]
            # print(a)
            wykres_disk_accesses[0].append(a[0])
            wykres_disk_accesses[1].append(a[2])
            wykres_phases[0].append(a[0])
            wykres_phases[1].append(a[3])

    plt.plot(wykres_disk_accesses[0], wykres_disk_accesses[1])
    # plt.legend(["real", "theo"])
    # plt.xscale('log')
    # plt.yscale('log')
    plt.title("Disk accesses vs number of records for block containing 16 records")
    plt.ylabel("Disk accesses")
    plt.xlabel("Number of records")
    fig = plt.gcf()
    fig.set_size_inches(15, 10)
    fig.savefig('disk_accesses.png', dpi=100)
    plt.show()

    plt.plot(wykres_disk_accesses[0], wykres_disk_accesses[1])
    # plt.legend(["real", "theo"])
    plt.xscale('log')
    plt.yscale('log')
    plt.title("Disk accesses vs number of records for block containing 16 records")
    plt.ylabel("Disk accesses")
    plt.xlabel("Number of records")
    fig = plt.gcf()
    fig.set_size_inches(15, 10)
    fig.savefig('disk_accesses_log.png', dpi=100)
    plt.show()

    plt.plot(wykres_phases[0], wykres_phases[1])
    # plt.xscale('log')
    # plt.yscale('log')
    plt.title("Phases vs number of records")
    plt.ylabel("Phases")
    plt.xlabel("Number of records")
    fig = plt.gcf()
    fig.set_size_inches(15, 10)
    fig.savefig('phases.png', dpi=100)
    plt.show()

    plt.plot(wykres_phases[0], wykres_phases[1])
    plt.xscale('log')
    # plt.yscale('log')
    plt.title("Phases vs number of records")
    plt.ylabel("Phases")
    plt.xlabel("Number of records")
    fig = plt.gcf()
    fig.set_size_inches(15, 10)
    fig.savefig('phases_lox.png', dpi=100)
    plt.show()