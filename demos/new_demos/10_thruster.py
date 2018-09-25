import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

cyl = sim.send_cylinder(position=(0, 0, 1))

thruster = sim.send_truster(cyl,
                            force_range=(5, 5))

sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)