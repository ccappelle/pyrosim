import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator()

sim.send_cylinder()

sim.send_box(position=(1, 0, 0))

sim.send_sphere(position=(-1, 0, 0))

sim.start()
sim.wait_to_finish()