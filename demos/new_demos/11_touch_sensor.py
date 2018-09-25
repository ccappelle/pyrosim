import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

cyl = sim.send_cylinder(position=(0, 0, 0.25 + 0.05))
touch = sim.send_touch_sensor(cyl)

sim.start()
sim.wait_to_finish()

print(sim.get_sensor_data(touch))