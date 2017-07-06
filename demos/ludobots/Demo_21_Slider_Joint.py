import sys
sys.path.insert(0, '../..')

import pyrosim
import math

# debug draws the slider joint as a green cylinder
sim = pyrosim.Simulator(eval_time=500, debug=True)

ARM_LENGTH = 0.5
ARM_RADIUS = ARM_LENGTH / 10.0

cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS,
                         r1=0, r2=0, r3=1,
                         length=ARM_LENGTH, radius=ARM_RADIUS)
cyl2 = sim.send_cylinder(x=0, y=0, z=1.5*ARM_LENGTH,
                         r1=0, r2=0, r3=1,
                         length=ARM_LENGTH, radius=ARM_RADIUS)

joint = sim.send_slider_joint(first_body_id=cyl1, second_body_id=cyl2,
                              x=0, y=0, z=1, lo=-.5, hi=.5,
                              strength=10,
                              position_control=True)

touch1 = sim.send_touch_sensor(body_id=cyl1)
touch2 = sim.send_touch_sensor(body_id=cyl1)

fneuron = sim.send_function_neuron(math.sin)

mneuron = sim.send_motor_neuron(joint)

sim.send_synapse(fneuron, mneuron, weight=-1.0)

sim.start()
sim.wait_to_finish()
