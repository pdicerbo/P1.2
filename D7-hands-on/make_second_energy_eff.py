import matplotlib.pyplot as plt
import numpy as np

# GFlops given by HPL
atlas_gflop_s = 3.716e+02
mkl_gflop_s = 4.745e+02

atlas_core = 207.3
atlas_dram = 75.5
# pkg - pp0
atlas_uncore = 242.845 - 207.3
atlas_total = atlas_core + atlas_dram + atlas_uncore

# MKL DATA
mkl_core = 217.25
mkl_dram = 59.692
# pkg - pp0
mkl_uncore = 252.084 - 217.25
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
plt.ylabel('Energy Efficiency (GFLOPS/w)')

# plt.show()
plt.savefig('watt_efficiency.png')
plt.close()
