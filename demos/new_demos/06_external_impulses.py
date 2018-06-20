import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1, play_paused=True, dt=0.001)

cyl = sim.send_cylinder(position=(0, 0, 1))
sim.add_impulse_to_body(cyl, (.01,0,.01))

sim.start()
sim.wait_to_finish()
print(sim._raw_cerr)