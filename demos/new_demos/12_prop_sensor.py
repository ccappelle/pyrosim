import sys
sys.path.insert(0, '../../')
import pyrosim
import matplotlib.pyplot as plt

sim = pyrosim.Simulator(eval_steps=300)

cyl = sim.send_cylinder(position=(0.5, 0, 2.0),
                        orientation=(1, 0, 0),
                        length=1.0
                        )

box = sim.send_box(position=(1.5, 0, 2.0) )

hinge = sim.send_hinge_joint(-1, cyl,
                             anchor=(0, 0, 2.0),
                             axis=(0, 1, 0))
slider = sim.send_slider_joint(-1, box,
                                axis=(0, 0, 1),
                                joint_range=1)

prop_hinge_sensor = sim.send_proprioceptive_sensor(hinge)
prop_slider_sensor = sim.send_proprioceptive_sensor(slider)
sim.start()
sim.wait_to_finish()

data1 = sim.get_sensor_data(prop_hinge_sensor)
data2 = sim.get_sensor_data(prop_slider_sensor)
# print(sim.get_sensor_data(prop_sensor))

plt.plot(data1)
plt.plot(data2)
plt.show()