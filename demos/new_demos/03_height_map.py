import sys
sys.path.insert(0, '../..')
import pyrosim

import numpy as np
import matplotlib.pyplot as plt

# height field is incomplete and needs work

# slanted plane
vec = np.linspace(0,1, 5)
mat = np.tile(vec, (5, 1))
z = 0
# stability issues around edges
# better to set them to 0
# by inverting height
mat[0, :] = -z
mat[:, 0] = -z
mat[-1, :] = -z
mat[:, -1] = -z

# pos = (0, 0, )
sim = pyrosim.Simulator(eval_steps=-1, play_paused=True, dt=0.001)

for i in range(10):
    sim.send_cylinder(position=(np.random.rand() * 10.0 - 5.0, 
                                np.random.rand() * 10.0 - 5.0,
                                2.0),
                      orientation=(0, 0, 1))

sim.send_height_map(mat,
                    position=(0, 0, z),
                    size=10)

sim.start()

# plt.imshow(mat)
# plt.show()

sim.wait_to_finish()
print(sim._strings_to_send)
print(sim._raw_cerr)

N, M = np.shape(mat)
vec = np.ravel(mat)