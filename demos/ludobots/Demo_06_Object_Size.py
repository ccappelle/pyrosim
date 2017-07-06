import sys
sys.path.insert(0, '../..')

import pyrosim

sim = pyrosim.Simulator()

sim.send_cylinder(x=0, y=0, z=0.1, r1=0, r2=1,
                  r3=0, length=2.0, radius=0.1)

sim.start()
sim.wait_to_finish()
