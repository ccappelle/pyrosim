import sys
sys.path.insert(0, '../..')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

body_id = sim.send_composite_body()

sim.add_box_to_composite(body_id)
sim.add_cylinder_to_composite(body_id)

sim.start()
sim.wait_to_finish()
