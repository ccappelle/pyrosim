import sys
sys.path.insert(0, '../../')
import pyrosim
import numpy as np

DT = 0.01
# toggle joint drawing by pressing 'd'
sim = pyrosim.Simulator(eval_steps=-1, play_paused=True, dt=DT)


cyl = sim.send_cylinder(position=(0.25, 0, 1),
                        orientation=(1, 0, 0),
                        length=0.5)
sphere = sim.send_sphere(position=(0.5, 0, 1),
                         radius=0.1)


hinge = sim.send_hinge_joint(-1, cyl,
                     anchor=(0, 0, 1),
                     axis=(0, 1, 0),
                     joint_range=np.pi/4.0)

slider = sim.send_slider_joint(cyl, sphere,
                      axis=(1, 0, 0),
                      joint_range=0.3)

rotary_motor = sim.send_rotary_actuator(hinge,
                                        control='positional',
                                        max_force=10)
linear_motor = sim.send_linear_actuator(slider)

# create sine wave as input
t_values = np.arange(0, np.pi * 2, DT)
input_values = 10 * np.sin(t_values)
input_neuron = sim.send_user_neuron(input_values)

# motor neurons correspondnig to actuators
rotary_neuron = sim.send_motor_neuron(rotary_motor)
linear_neuron = sim.send_motor_neuron(linear_motor)

sim.send_synapse(input_neuron, rotary_neuron, 1.0)
sim.send_synapse(input_neuron, linear_neuron, 1.0)

sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)