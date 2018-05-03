import sys
sys.path.insert(0, '../..')
import pyrosim

import numpy as np
import matplotlib.pyplot as plt

# height field is incomplete and needs work

# saddle
vec = np.linspace(-1, 1, 9)
mat = np.outer(vec, vec[:5])

sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)

body_id = sim.send_cylinder(position=(0.5, 0.5, 3),
                            orientation=(-1, 1, 0))

sim.send_height_map(mat,
                    position=(0, 0, 1.0), # lift up so out of the ground
                    dimensions=(10, 10))

sim.start()

# plt.imshow(mat)
# plt.show()

sim.wait_to_finish()
