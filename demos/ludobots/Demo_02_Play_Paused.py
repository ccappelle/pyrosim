import sys
sys.path.insert(0, '..')

import pyrosim


sim = pyrosim.Simulator(play_paused=True)

sim.send_cylinder()

sim.start()  # press ctrl-p (even on mac) to unpause
sim.wait_to_finish()
