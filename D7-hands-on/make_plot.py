import matplotlib.pyplot as plt
import numpy as np

atlas_gflop_s = 3.716e+02
mkl_gflop_s = 4.745e+02

mkl_walltime = 795.21
atlas_walltime = 1015.55

libs = ('mkl-gnu', 'atlas')
y_pos = np.zeros(2)
y_pos[0] = mkl_gflop_s; y_pos[1] = atlas_gflop_s
x_pos = np.arange(len(y_pos))
bar_width = 0.35

plt.bar(x_pos, y_pos, bar_width, align='center')
plt.xticks(x_pos, libs)
plt.xlabel('library')
plt.ylabel('GFLOPS/s')

plt.savefig('n_operation.png')
plt.close()
