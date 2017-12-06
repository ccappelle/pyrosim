import sys
sys.path.insert(0, '../..')

import pyrosim


ARM_LENGTH = 0.5
ARM_RADIUS = ARM_LENGTH / 10.0
PI = 3.14159

sim = pyrosim.Simulator(eval_time=500, debug=True)

cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS,
                         r1=0, r2=0, r3=1,
                         length=ARM_LENGTH, radius=ARM_RADIUS)
cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS,
                         r1=0, r2=1, r3=0,
                         length=ARM_LENGTH, radius=ARM_RADIUS)

joint = sim.send_hinge_joint(first_body_id=cyl1, second_body_id=cyl2,
                             x=0, y=0, z=(ARM_LENGTH + ARM_RADIUS),
                             n1=1, n2=0, n3=0, lo=-PI/4.0, hi=+PI/4.0)

touch1 = sim.send_touch_sensor(cyl1)
touch2 = sim.send_touch_sensor(cyl2)

sneuron1 = sim.send_sensor_neuron(touch1)
sneuron2 = sim.send_sensor_neuron(touch2)

mneuron = sim.send_motor_neuron(joint)

# weights are opposite 
sim.send_synapse(sneuron1,mneuron, weight=1.0)
sim.send_synapse(sneuron2,mneuron, weight=-.5)

sim.start()
sim.wait_to_finish()



