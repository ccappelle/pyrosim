import sys
sys.path.insert(0, '../../')
import pyrosim
import numpy as np


sim = pyrosim.Simulator(play_paused=True, eval_steps=-1)

length = 0.5
radius = length / 5.0

# create cylinders in right angle arm
cyl1 = sim.send_cylinder(position=(0, 0, length / 2.0 + radius),
                         length=length,
                         radius=radius)
cyl2 = sim.send_cylinder(position=(length / 2.0, 0, length + radius),
                         orientation=(1, 0, 0),
                         length=length,
                         radius=radius)

box = sim.send_box(position=(5 *length, 0, length),
                   sides=(2 * length, 2 * length, 2* length),
                   color=(1, 0, 1)
                   )
# # joint cylinders
joint = sim.send_hinge_joint(cyl1, cyl2,
                             anchor=(0, 0, length + radius),
                             axis=(0, 1, 0),
                             joint_range=(-3.14159 / 2.0, +3.14159 / 2.0)
                             )

# # create motor
motor = sim.send_rotary_actuator(joint,
                                 max_force=100.0, # max amount of force able to be used
                                 speed=1.0,       # speed multiplier of motor
                                 control='positional'  # how the motor moves based on input
                                 )
ray = sim.send_ray(cyl2,
                   position=(length, 0, length + radius),
                   orientation=(1, 0, 0),
                   max_length=10.0)

x_sensor = sim.send_position_sensor(cyl1, 'x')
ray_sensor = sim.send_ray_sensor(ray, 'r')

sensor_neuron = sim.send_sensor_neuron(x_sensor)
sensor_neuron2 = sim.send_sensor_neuron(ray_sensor)

motor_neuron = sim.send_motor_neuron(motor)
sim.send_synapse(sensor_neuron, motor_neuron, 10.0)
sim.send_synapse(sensor_neuron2, motor_neuron, -1.0)
sim.start()
sim.wait_to_finish()

ray_sensor_out = sim.get_sensor_data(ray_sensor)

print(ray_sensor_out)