import numpy as np
import matplotlib.pyplot as plt
# import sys.argv as ar

# if len(ar) < 2:
#     print "\n\tUsage: %s namefile\n" % ar[0]
# else:
#     print "\n\tOk\n"

print "\nMaking plot\n"

matrix = np.loadtxt("output.txt", dtype=float)
ncores = matrix[:,0]
band   = matrix[:,1]

plt.figure()
plt.plot(ncores, band)
# plt.show()
plt.savefig("plot.png")
