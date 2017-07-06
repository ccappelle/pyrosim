import sys
sys.path.insert(0, '../..')

import pyrosim


sim = pyrosim.Simulator()

x = 0
y = 0
z = 0.1

# no collisions so everything 'collapses'
for i in range(0, 10):
    sim.send_cylinder(x=x, y=y, z=z, r1=0, r2=1,
                      r3=0, length=1.0, radius=0.1)
    z = z + 2.0 * 0.1

sim.start()
sim.wait_to_finish()
