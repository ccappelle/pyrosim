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

# joint cylinders
joint = sim.send_hinge_joint(cyl1, cyl2,
                             anchor=(0, 0, length + radius),
                             axis=(0, 1, 0),
                             # joint_range=(-3.14159 / 2.0, +3.14159 / 2.0)
                             )

# create motor
motor = sim.send_rotary_actuator(joint,
                                 max_force=100.0, # max amount of force able to be used
                                 speed=3.0,       # speed multiplier of motor
                                 control='positional'  # how the motor moves based on input
                                 )

t = np.linspace(0, 2 * np.pi, num=50)
inputs = 10.0 * np.sin(t)

input_neuron = sim.send_user_neuron(inputs)
motor_neuron = sim.send_motor_neuron(motor, alpha=1.0, tau=0.1)

sim.send_synapse(input_neuron, motor_neuron, 10.0)

sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)