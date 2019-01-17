import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator( eval_steps = -1 )


box1 = sim.send_box( position= (  1, 0, 2) )
box2 = sim.send_box( position= (  1, 0, 1) )

sim.set_gravity( z=0.0 )

sim.send_spring( box1, box2, resting_length = 0.5, stiffness = 0.2, damping = 0.1 )

sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)