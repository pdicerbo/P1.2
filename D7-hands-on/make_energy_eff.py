import matplotlib.pyplot as plt
import numpy as np

# GFlops given by HPL
atlas_gflop_s = 3.716e+02
mkl_gflop_s = 4.745e+02

#ENERGY DATA

#ATLAS
atlas_core = 214583.001
atlas_dram = 78152.569
atlas_pkg  = 251377.688
# pkg - pp0
atlas_uncore = atlas_pkg - atlas_core
atlas_total = atlas_core + atlas_dram + atlas_uncore

# MKL
mkl_core = 177081.552
mkl_dram = 48654.667
mkl_pkg  = 205475.494
# pkg - pp0
mkl_uncore = mkl_pkg - mkl_core
mkl_total = mkl_core + mkl_dram + mkl_uncore

libs = ('mkl-gnu', 'atlas')
y_pos = np.zeros(2)
y_pos[0] = mkl_gflop_s / mkl_total; y_pos[1] = atlas_gflop_s / atlas_total
x_pos = np.arange(len(y_pos))
bar_width = 0.35

p1 = plt.bar(x_pos, y_pos, bar_width, align='center', color = 'r')

plt.xticks(x_pos, libs)
plt.title('ENERGY EFFICIENCY')
plt.xlabel('library')
plt.ylabel('Energy Efficiency (GFLOPS/J)')

plt.show()
# plt.savefig('power_cons.png')
plt.close()
