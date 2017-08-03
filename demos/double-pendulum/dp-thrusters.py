import pyrosim
import math

sim = pyrosim.Simulator(play_paused=True, debug=True, use_textures=True,
                        xyz=[3.0, 2.0, 2.0], hpr=[-150.0, -15.0, 0.0],
                        eval_time=1000)

cyl_1 = sim.send_cylinder(x=-0.25, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)
cyl_2 = sim.send_cylinder(x=-0.75, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)

box = sim.send_box(x=-1.0, y=0.0, z=2.0,
                   width=0.5, length=0.5, height=0.5,
                   mass=10.0)

world_joint = sim.send_hinge_joint(pyrosim.Simulator.WORLD, cyl_1,
                                   x=0.0, y=0.0, z=2.0,
                                   n1=0, n2=1, n3=0,
                                   position_control=False,
                                   torque=1000, speed=3)

hinge_joint = sim.send_hinge_joint(cyl_1, cyl_2,
                                   x=-0.5, y=0.0, z=2.0,
                                   n1=0, n2=1, n3=0,
                                   position_control=False,
                                   torque=1000, speed=3)

slider_joint = sim.send_slider_joint(cyl_2, box,
                                     x=1.0, y=0.0, z=0.0,
                                     lo=-0.5, hi=0.5)

# thrusters are connected to a body (here the 'box')
# and will rotate as this body rotates in space
# they are given direction in global coordinates (x,y,z)
# here the motor is on the 'bottom' of the box
thruster = sim.send_thruster(box, x=-1, y=0, z=0, hi=10, lo=-10)

# network
bneuron = sim.send_bias_neuron()
mneuron = sim.send_motor_neuron(thruster, alpha=0)
sim.send_synapse(bneuron, mneuron, weight=1.0)

sim.start()
results = sim.wait_to_finish()
