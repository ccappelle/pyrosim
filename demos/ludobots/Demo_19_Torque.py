import sys
sys.path.insert(0, '../..')

import pyrosim
import math


ARM_LENGTH = 0.75
ARM_RADIUS = ARM_LENGTH / 10.0

TORQUES = [0.1, 100.0]
SPEEDS = [0.1, 1.0]

# torque is max torque possible, not neccesarily the torque used
for torque in TORQUES:
    for speed in SPEEDS:
        sim = pyrosim.Simulator(eval_time=100)

        cyl = sim.send_cylinder(x=0, y=0, z=2.0*ARM_LENGTH,
                                r1=0, r2=0, r3=1,
                                length=ARM_LENGTH, radius=ARM_RADIUS)

        box = sim.send_box(x=0, y=0, z=1.25*ARM_LENGTH, length=ARM_RADIUS *
                           7., width=ARM_RADIUS*7.0, height=ARM_RADIUS*7.0)

        world_cyl_joint = sim.send_hinge_joint(
            first_body_id=-1, second_body_id=cyl,
            x=0, y=0, z=2.5*ARM_LENGTH,
            n1=1, n2=0, n3=0, lo=-3.14159/2.0, hi=+3.14159/2.0,
            torque=torque, speed=speed, position_control=True
        )

        cyl_box_joint = sim.send_hinge_joint(
            first_body_id=cyl, second_body_id=box,
            x=0, y=0, z=1.5*ARM_LENGTH)

        fneuron = sim.send_user_input_neuron(in_values=1)
        mneuron = sim.send_motor_neuron(joint_id=world_cyl_joint)

        sim.send_synapse(source_neuron_id=fneuron,
                         target_neuron_id=mneuron, weight=1.0)

        sim.film_body(box, 'track')
        sim.start()
        sim.wait_to_finish()
