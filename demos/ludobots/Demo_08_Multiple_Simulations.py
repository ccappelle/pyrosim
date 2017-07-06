import sys
sys.path.insert(0, '../..')

import pyrosim

NUM_SIMS = 5
sims = [0]*NUM_SIMS
for s in range(NUM_SIMS):
    sims[s] = pyrosim.Simulator()
    sims[s].send_cylinder(x=0, y=0, z=0.1, r1=0, r2=1,
                          r3=0, length=1.0, radius=0.1)
    sims[s].start()

for s in range(NUM_SIMS):
    sims[s].wait_to_finish()

