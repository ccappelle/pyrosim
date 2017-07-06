import sys
sys.path.insert(0, '../..')

import pyrosim


sim = pyrosim.Simulator(eval_time=1000)

sim.send_cylinder()

sim.start()
sim.wait_to_finish()
