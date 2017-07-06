import sys
sys.path.insert(0, '../..')

import pyrosim

# use_textures draws the default ODE textures for ground
# and sky
sim = pyrosim.Simulator(eval_time=500, use_textures=True)

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
                   mass=10)

axles = [0]*4
count = 0

for x_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
    for y_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
        # position_control = False -> continuous range of motion
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
    mneurons[i] = sim.send_motor_neuron(axles[i])
    sim.send_synapse(bias, mneurons[i], weight=-1.0)

sim.film_body(box, method='follow')

sim.start()
sim.wait_to_finish()

