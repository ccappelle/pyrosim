import sys
sys.path.insert(0, '../..')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

body_id = sim.send_composite_body()
sim.add_sphere_to_composite(body_id, position=(0, 0, 0))
sim.add_cylinder_to_composite(body_id, position=(0, 0, 1))
sim.add_box_to_composite(body_id, position=(0, 0, 2))

sim.start()
sim.wait_to_finish()