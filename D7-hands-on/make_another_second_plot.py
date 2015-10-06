import matplotlib.pyplot as plt
import numpy as np

mkl_alltime = 795.21
atlas_alltime = 1015.55

# ATLAS DATA
atlas_core = 214583.001 / atlas_alltime
atlas_dram = 78152.569 / atlas_alltime
atlas_pkg  = 251377.688 / atlas_alltime
# pkg - pp0
atlas_uncore = atlas_pkg - atlas_core
atlas_total = atlas_core + atlas_dram + atlas_uncore

# MKL DATA
mkl_core = 177081.552 / mkl_alltime
mkl_dram = 48654.667 / mkl_alltime
mkl_pkg  = 205475.494 / mkl_alltime
# pkg - pp0
mkl_uncore = mkl_pkg - mkl_core
mkl_total = mkl_core + mkl_dram + mkl_uncore

libs = ('mkl-gnu', 'atlas')
y_pos = np.zeros(2)
y_pos[0] = mkl_total; y_pos[1] = atlas_total
x_pos = np.arange(len(y_pos))
bar_width = 0.35

p1 = plt.bar(x_pos, y_pos, bar_width, align='center', color = 'r')

y_pos[0] = mkl_dram + mkl_uncore; y_pos[1] = atlas_dram + atlas_uncore

p2 = plt.bar(x_pos, y_pos, bar_width, align='center', color = 'g')

y_pos[0] = mkl_uncore; y_pos[1] = atlas_uncore

p3 = plt.bar(x_pos, y_pos, bar_width, align='center', color = 'b')

plt.xticks(x_pos, libs)
plt.title('AVERAGE POWER')
plt.xlabel('library')
plt.ylabel('Average Power (W)')
plt.legend( (p1[0], p2[0], p3[0]), ('Core', 'Dram', 'Uncore') )
plt.show()
# plt.savefig('avg_power.png')
plt.close()
