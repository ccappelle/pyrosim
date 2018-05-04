import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

cyl = sim.send_cylinder(position=(0.25, 0, 1),
                        orientation=(1, 0, 0),
                        length=0.5)

# sim.send_hinge_joint(cyl, -1,
#                      anchor=(0, 0, 1),
#                      axis=(0, 1, 0))
sim.send_slider_joint(cyl, -1, axis=(0, 0, 1),
                      )

sim.start()
sim.wait_to_finish()

print(sim._strings_to_send)
print(sim._raw_cerr)