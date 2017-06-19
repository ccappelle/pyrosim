import sys
sys.path.insert(0, '..')

import pyrosim
import math


def run(test):

    if test:
        kwargs = {'debug': False, 'play_blind': True, 'eval_time': 500}
    else:
        kwargs = {'debug': True, 'play_blind': False, 'eval_time': 500,
                  'xyz': [-7, 0, 1]}

    sim = pyrosim.Simulator(**kwargs)

    WHEEL_RADIUS = 0.1
    SPEED = 10

    wheels = [0]*4
    count = 0
    for x_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
        for y_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
            wheels[count] = sim.send_sphere(
                x=x_pos, y=y_pos, z=WHEEL_RADIUS, radius=WHEEL_RADIUS)
            count += 1

    box = sim.send_box(x=0, y=0, z=1.5*WHEEL_RADIUS, length=4 *
                       WHEEL_RADIUS, width=5*WHEEL_RADIUS, height=WHEEL_RADIUS,
                       mass=2)

    # position_control = False -> continuous range of motion
    axles = [0]*4
    count = 0
    for x_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
        for y_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
            axles[count] = sim.send_hinge_joint(first_body_id=wheels[count],
                                                second_body_id=box, x=x_pos,
                                                y=y_pos, z=WHEEL_RADIUS,
                                                n1=1, n2=0, n3=0,
                                                position_control=False,
                                                speed=SPEED)
            count += 1

    bias = sim.send_bias_neuron()

    mneurons = [0]*4
    for i in range(4):
        mneurons[i] = sim.send_motor_neuron(joint_id=axles[i])
        sim.send_synapse(source_neuron_id=bias,
                         target_neuron_id=mneurons[i], weight=1.0)

    sim.send_box(x=0, y=-3, z=.5, length=1, height=1, width=1, mass=5,
                 collision_group='env', r=.5, g=0, b=0)
    sphere = sim.send_sphere(x=0, y=-3, z=1+.5, radius=0.5,
                             collision_group='env', mass=3,
                             r=0, g=.5, b=0)
    sim.send_cylinder(x=0, y=-3, z=2+.5+0.2, mass=2, r1=0, r2=0, r3=1,
                      length=1.0, radius=0.2, collision_group='env',
                      r=0, g=0, b=.5)

    sim.send_collision_matrix('all')
    sim.film_body(sphere, 'track')
    sim.start()
    sim.wait_to_finish()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(test=True)
        print('Successfully made standard collision')
    else:
        run(test=False)
