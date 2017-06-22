import sys
sys.path.insert(0, '../..')

import pyrosim


def run(test):

    if test:
        kwargs = {'debug': False, 'play_blind': True, 'eval_time': 500}
    else:
        kwargs = {'debug': True, 'play_blind': False, 'eval_time': 500}

    ARM_LENGTH = 0.5
    ARM_RADIUS = ARM_LENGTH / 10.0

    sim = pyrosim.Simulator(**kwargs)

    cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS,
                             r1=0, r2=0, r3=1,
                             length=ARM_LENGTH, radius=ARM_RADIUS)

    cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS,
                             r1=0, r2=1, r3=0,
                             length=ARM_LENGTH, radius=ARM_RADIUS)

    sim.send_hinge_joint(first_body_id=cyl1, second_body_id=cyl2,
                         x=0, y=0, z=ARM_LENGTH + ARM_RADIUS,
                         n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

    # Touch sensors return 1 or 0 depending on whether the body is
    # touching or not. Value is stored at svi=0
    sensor1 = sim.send_touch_sensor(body_id=cyl1)
    sensor2 = sim.send_touch_sensor(body_id=cyl2)

    sim.start()
    results = sim.wait_to_finish()

    if not test:
        print 'All sensor results'
        print results,'\n'
        print 'Selected sensor results'
        print 'Touch sensor 1:'
        print sim.get_sensor_data(sensor1, svi=0)
        print 'Touch sensor 2:'
        print sim.get_sensor_data(sensor2, svi=0)


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(True)
        print('Successfully made touch sensors')
    else:
        run(False)
