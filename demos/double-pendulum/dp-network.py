import pyrosim
import math

sim = pyrosim.Simulator(play_paused=True, debug=True, use_textures=True,
                        xyz=[3.0, 2.0, 2.0], hpr=[-150.0, -15.0, 0.0],
                        eval_time=1000)

# cylinders start with long axis along the (r1,r2,r3) axis
# r1=1,r2=0, r3=0 means it is starting pointed along the x-axis
cyl_1 = sim.send_cylinder(x=-0.25, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)
cyl_2 = sim.send_cylinder(x=-0.75, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)

box = sim.send_box(x=-1.0, y=0.0, z=2.0,
                   width=0.5, length=0.5, height=0.5,
                   mass=10.0)

# hinge connects body to the world
# (x,y,z) refers to the anchor position
# (n1,n2,n3) refers to the joint axis
world_joint = sim.send_hinge_joint(first_body_id=pyrosim.Simulator.WORLD,
                                   second_body_id=cyl_1,
                                   x=0.0, y=0.0, z=2.0,
                                   n1=0, n2=1, n3=0,
                                   position_control=False,
                                   torque=1000, speed=3)

# hinge connects cylinders together
hinge_joint = sim.send_hinge_joint(first_body_id=cyl_1,
                                   second_body_id=cyl_2,
                                   x=-0.5, y=0.0, z=2.0,
                                   n1=0, n2=1, n3=0,
                                   position_control=False,
                                   torque=1000, speed=3)

# slider joint joins second cylinder and box.
# Contrary to the hinge joint, the parameters
# (x,y,z) refer to the joint axis in the case
# of the slider joint.
slider_joint = sim.send_slider_joint(first_body_id=cyl_2,
                                     second_body_id=box,
                                     x=1.0, y=0.0, z=0.0,
                                     lo=-0.5, hi=0.5)

pos_sensor = sim.send_position_sensor(cyl_1)
vestib_sensor = sim.send_vestibular_sensor(cyl_2)
prop_sensor = sim.send_proprioceptive_sensor(slider_joint)

# network is designed to actuate hinge joints in
# response to the slider joint moving
bias_neuron = sim.send_bias_neuron()
sensor_neuron = sim.send_sensor_neuron(prop_sensor)
motor_1_neuron = sim.send_motor_neuron(world_joint)
motor_2_neuron = sim.send_motor_neuron(hinge_joint)

# simple synapses connect sensor to motor
# bias neurons get the ball rolling
sim.send_synapse(bias_neuron, motor_1_neuron, .001)
sim.send_synapse(bias_neuron, motor_2_neuron, .001)
sim.send_synapse(sensor_neuron, motor_1_neuron, 1.0)
sim.send_synapse(sensor_neuron, motor_2_neuron, 1.0)

sim.start()
results = sim.wait_to_finish()