import matplotlib.pyplot as plt
import numpy as np

# PLOT AVERAGE POWER CONSUMPTION

# ATLAS DATA (AVERAGE POWER CONSUMPTION)
atlas_core = 207.3
atlas_dram = 75.5
# pkg - pp0
atlas_uncore = 242.845 - 207.3
atlas_total = atlas_core + atlas_dram + atlas_uncore

# MKL DATA (AVERAGE POWER CONSUMPTION)
mkl_core = 217.25
mkl_dram = 59.692
# pkg - pp0
mkl_uncore = 252.084 - 217.25
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
plt.title('POWER CONSUMPTION')
plt.xlabel('library')
plt.ylabel('Power Usage (W)')
plt.legend( (p1[0], p2[0], p3[0]), ('Core', 'Dram', 'Uncore') )
# plt.show()
plt.savefig('power_cons.png')
plt.close()

