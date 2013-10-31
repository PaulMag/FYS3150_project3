import matplotlib.pyplot as plt
import numpy as np
from sys import argv

folder = argv[1]

infile = open("data/%s/info.dat" % folder, "r")

info = infile.readline().split(",")
time, n, dim = float(info[0]), int(info[1]), int(info[2])

names = infile.readline()[:-2].split(",") # (avoid last comma)
N = len(names)

infile.close()

positions = np.zeros((N, n, dim))

for i in range(N):
    infile = open("data/%s/obj%d.dat" % (folder,i), "r")

    for j in range(n):
        line = infile.readline().split(",")
        for k in range(dim):
            positions[i,j,k] = float(line[k])

    infile.close()


plt.figure()
plt.title(folder + ", time = %g years, h = %g years" % (time, time/n))
plt.axis("equal")
plt.xlabel("x [AU]"); plt.ylabel("y [AU]")
plt.grid('on')

for i in range(N):
    plt.plot( positions[i,:,0], positions[i,:,1] )
plt.legend( names, loc="best" )

plt.show()
