import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)

cyl = sim.send_cylinder(position=(1, 0, 2))

sim.send_hinge_joint( -1, cyl,
                     anchor= (0, 0, 2 ),
                     axis=( 0, 1, 0) )

thruster = sim.send_truster(cyl,
                            force_range=(5, 5))

sim.start()
sim.wait_to_finish()

print( sim._raw_cerr )