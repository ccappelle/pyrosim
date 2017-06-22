import sys
sys.path.insert(0, '../..')

import pyrosim
import math


def run(test):

    if test:
        kwargs = {'debug': False, 'play_blind': True, 'eval_time': 500}
    else:
        kwargs = {'debug': True, 'play_blind': False, 'eval_time': 500}

    sim = pyrosim.Simulator(**kwargs)

    ARM_LENGTH = 0.5

    ARM_RADIUS = ARM_LENGTH / 10.0

    cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + 2*ARM_RADIUS,
                             r1=0, r2=0, r3=1,
                             length=ARM_LENGTH, radius=ARM_RADIUS)

    cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + 2 *
                             ARM_RADIUS, r1=0, r2=1, r3=0,
                             length=ARM_LENGTH, radius=ARM_RADIUS)

    cyl_cyl_joint = sim.send_hinge_joint(first_body_id=cyl1,
                                         second_body_id=cyl2,
                                         x=0, y=0,
                                         z=ARM_LENGTH + 2*ARM_RADIUS,
                                         n1=1, n2=0, n3=0,
                                         lo=-3.14159/4.0, hi=+3.14159/4.0)

    cyl_world_joint = sim.send_hinge_joint(
        first_body_id=cyl1,
        second_body_id=pyrosim.Simulator.WORLD,
        x=0, y=0,
        z=ARM_LENGTH/2.0 + 2*ARM_RADIUS)

    fneuron = sim.send_function_neuron(function=math.sin)
    mneuron = sim.send_motor_neuron(joint_id=0)

    # developing synapse changes over evaluation time
    sim.send_developing_synapse(source_neuron_id=fneuron,
                                target_neuron_id=mneuron,
                                start_weight=1.0, end_weight=-1.0,
                                start_time=0, end_time=1.0)
    sim.start()
    sim.wait_to_finish()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(test=True)
        print('Successfully made changing synapse')
    else:
        run(test=False)
