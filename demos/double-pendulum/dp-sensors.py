import pyrosim
import math

sim = pyrosim.Simulator(play_paused=True, debug=True, use_textures=True,
                        xyz=[3.0, 2.0, 2.0], hpr=[-150.0, -25.0, 0.0],
                        eval_time=500)

# cylinders start with long axis along the (r1,r2,r3) axis
# r1=1,r2=0, r3=0 means it is starting pointed along the x-axis
cyl_1 = sim.send_cylinder(x=-0.25, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)
cyl_2 = sim.send_cylinder(x=-0.75, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)

box = sim.send_box(x=-1.0, y=0.0, z=2.0,
                   width=0.5, length=0.5, height=0.5,
                   mass=10.0)

# hinge connects body to the world
# (x,y,z) refers to the anchor position
# (n1,n2,n3) refers to the joint axis
world_joint = sim.send_hinge_joint(first_body_id=pyrosim.Simulator.WORLD,
                                     second_body_id=cyl_1,
                                     x=0.0, y=0.0, z=2.0,
                                     n1=0, n2=1, n3=0,
                                     lo=-math.pi, hi=math.pi)

# hinge connects cylinders together
hinge_joint = sim.send_hinge_joint(first_body_id=cyl_1,
                                     second_body_id=cyl_2,
                                     x=-0.5, y=0.0, z=2.0,
                                     n1=0, n2=1, n3=0,
                                     lo=-math.pi, hi=math.pi)

# slider joint joins second cylinder and box.
# Contrary to the hinge joint, the parameters
# (x,y,z) refer to the joint axis in the case
# of the slider joint.
slider_joint = sim.send_slider_joint(first_body_id=cyl_2,
                                      second_body_id=box,
                                      x=1.0, y=0.0, z=0.0,
                                      lo=-0.5,hi=0.5)

pos_sensor = sim.send_position_sensor(cyl_1)
vestib_sensor = sim.send_vestibular_sensor(cyl_2)
prop_sensor = sim.send_proprioceptive_sensor(slider_joint)

sim.start()
results = sim.wait_to_finish()

# we can access the results matrix directly
print(str(results) + '\n')

# or use built in commands to get a specific sensor and svi
# value for every time step

pos_x_sensor_results = sim.get_sensor_data(pos_sensor, svi=0)
pos_y_sensor_results = sim.get_sensor_data(pos_sensor, svi=1)
pos_z_sensor_results = sim.get_sensor_data(pos_sensor, svi=2)
vestib_sensor_results = sim.get_sensor_data(vestib_sensor)
prop_sensor_results = sim.get_sensor_data(prop_sensor)

for t in range(500):
    x = pos_x_sensor_results[t]
    y = pos_y_sensor_results[t]
    z = pos_z_sensor_results[t]
    v = vestib_sensor_results[t]
    p = prop_sensor_results[t]

    output = ('{:3d}:: x:{: 3.1f}, y:{: 3.1f}, z:{: 3.1f}, ' +
              'vestib:{: 3.1f}, prop:{: 3.1f}').format(t, x, y, z, v, p)
    print(output)

