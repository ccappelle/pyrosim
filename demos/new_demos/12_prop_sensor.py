import sys
sys.path.insert(0, '../../')
import pyrosim
import matplotlib.pyplot as plt

sim = pyrosim.Simulator(eval_steps=300)

cyl = sim.send_cylinder(position=(0.5, 0, 2.0),
                        orientation=(1, 0, 0),
                        length=1.0
                        )

hinge = sim.send_hinge_joint(-1, cyl,
                             anchor=(0, 0, 2.0),
                             axis=(0, 1, 0))

prop_sensor = sim.send_proprioceptive_sensor(hinge)

sim.start()
sim.wait_to_finish()

data = sim.get_sensor_data(prop_sensor)
# print(sim.get_sensor_data(prop_sensor))

plt.plot(data)
plt.show()