import numpy as np
import matplotlib.pyplot as plt


matrix = np.loadtxt("numa00_output.txt")
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

matrix = np.loadtxt("numa01_output.txt")
ncores = matrix[:,0]
band   = matrix[:,1]

NMes = 10; i = 0; j = 0; k = 0;

mesures2 = np.zeros(6)

while i < len(band):
    tmp = 0.
    for j in range(NMes):
        tmp += band[j + i]

    mesures2[k] = tmp / NMes
    i += NMes; k += 1

plt.figure()
plt.plot(n_c, mesures, '-o')
plt.plot(n_c, mesures2, '-o')
plt.xlabel('Number of cores')
plt.ylabel('Bandwidth in MB/s')
plt.savefig("numa_plot_smallvec.png")
