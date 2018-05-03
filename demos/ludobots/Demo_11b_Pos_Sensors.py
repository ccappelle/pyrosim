import sys
sys.path.insert(0, '../..')

import pyrosim


ARM_LENGTH = 0.5
ARM_RADIUS = ARM_LENGTH / 10.0

sim = pyrosim.Simulator(eval_time=500)

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

for t in range(500):
    print('xyz position at time ', t, ' : ', x_data[t], y_data[t], z_data[t])
