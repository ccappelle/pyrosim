import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator()

sim.send_cylinder()

sim.send_box(position=(1, 0, 0))
sim.send_sphere(position=(-1, 0, 0))
sim.set_gravity(z=-9.8)
sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)