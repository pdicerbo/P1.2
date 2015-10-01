import numpy as np
import matplotlib.pyplot as plt


matrix = np.loadtxt("output.txt")
ncores = matrix[:,0]
band   = matrix[:,1]

NMes = 10; i = 0; j = 0; k = 0;

n_c = np.zeros(6)
mesures = np.zeros(6)

while i < len(band):
    tmp = 0.
    for j in range(NMes):
        tmp += band[j + i]

    mesures[k] = tmp / NMes
    n_c[k] = ncores[i]
    i += NMes; k += 1

plt.figure()
plt.plot(n_c, mesures, '-o')
plt.xlabel('Number of cores')
plt.ylabel('Bandwidth in MB/s')
plt.savefig("plot.png")
