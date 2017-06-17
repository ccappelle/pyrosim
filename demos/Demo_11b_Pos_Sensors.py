import sys
sys.path.insert(0, '..')

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

    sensor1 = sim.send_touch_sensor(body_id=cyl1)
    sensor2 = sim.send_touch_sensor(body_id=cyl2)

    # position sensors return x,y,z position stored
    # at svi=0,1,2 respectively
    position = sim.send_position_sensor(body_id=cyl2)

    sim.start()
    results = sim.wait_to_finish()

    x_data = sim.get_sensor_data(position, svi=0)
    y_data = sim.get_sensor_data(position, svi=1)
    z_data = sim.get_sensor_data(position, svi=2)

    if not test:
        for t in range(kwargs['eval_time']):
            print 'xyz position at time ', t,
            print ' : ', x_data[t], y_data[t], z_data[t]
    else:
        assert abs(x_data[-1]-0.193812) < 10**(-6), (
            'last x = ' + str(x_data[-1]) + ' should equal 0.193812')
        assert abs(y_data[-1]-0.322152) < 10**(-6), (
            'last y = ' + str(y_data[-1]) + ' should equal 0.322152')
        assert abs(z_data[-1]-0.206516) < 10**(-6), (
            'last z = ' + str(z_data[-1]) + ' should equal 0.206516')

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(test=True)
        print('Successfully made position sensor')
    else:
        run(test=False)
