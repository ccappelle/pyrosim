import pyrosim

WHEEL_RADIUS = 0.1
SPEED = 10

sim = pyrosim.Simulator(play_paused=True, eval_time=500, debug=True,
                        xyz=[5, -3, WHEEL_RADIUS*5],
                        hpr=[180, 10, 0], use_textures=True)

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
                                            lo=-3.14159/4.0, hi=+3.14159/4.0,
                                            position_control=False,
                                            speed=SPEED)
        count += 1

bias = sim.send_bias_neuron()

mneurons = [0]*4
for i in range(4):
    mneurons[i] = sim.send_motor_neuron(joint_id=axles[i])
    sim.send_synapse(source_neuron_id=bias,
                     target_neuron_id=mneurons[i], weight=1.0)

# default collision group is 0 so the bodies in the car have group 0
sim.send_box(x=0,y=-3,z=.5, length=1, height=1, width=1, mass=5,
                collision_group=1)
sphere = sim.send_sphere(x=0,y=-3,z=1+.5,radius=0.5,collision_group=2,mass=3)
sim.send_cylinder(x=0,y=-3,z=2+.5+0.2,mass=2,r1=0,r2=0,r3=1,
                    length=1.0,radius=0.2,collision_group=3)

sim.film_body(sphere,'track')
sim.start()
sim.wait_to_finish()
