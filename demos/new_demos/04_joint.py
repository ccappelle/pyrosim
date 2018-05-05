import sys
sys.path.insert(0, '../../')
import pyrosim

# toggle joint drawing by pressing 'd'
sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)

cyl = sim.send_cylinder(position=(0.25, 0, 1),
                        orientation=(1, 0, 0),
                        length=0.5)
sim.send_hinge_joint(-1, cyl,
                     anchor=(0, 0, 1),
                     axis=(0, 1, 0),
                     joint_range=None)

sim.start()
sim.wait_to_finish()