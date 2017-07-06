import sys
sys.path.insert(0, '../..')

import pyrosim

sim = pyrosim.Simulator()

sim.send_cylinder()

sim.start()
sim.wait_to_finish()
