import pyrosim

WHEEL_RADIUS = 0.1
SPEED = 10

sim = pyrosim.Simulator(play_paused=True, eval_time=1000, debug=True, xyz=[0,WHEEL_RADIUS*2.0,WHEEL_RADIUS*4], hpr=[-90,-10,0], use_textures=True)


wheels =[0]*4
count = 0
for x_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
	for y_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
		wheels[count] = sim.send_sphere(x=x_pos,y=y_pos, z=WHEEL_RADIUS, radius=WHEEL_RADIUS)
		count+=1

box = sim.send_box(x=0,y=0,z=WHEEL_RADIUS, length=4*WHEEL_RADIUS, width=4*WHEEL_RADIUS, height=WHEEL_RADIUS)

#position_control = False -> continuous range of motion
axles = [0]*4
count = 0
for x_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
	for y_pos in [-2*WHEEL_RADIUS, 2*WHEEL_RADIUS]:
		axles[count] = sim.send_hinge_joint(first_body_id=wheels[count], second_body_id=box, x=x_pos, 
						y=y_pos, z=WHEEL_RADIUS, n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0, 
						position_control = False, speed=SPEED)
		count+=1

bias = sim.send_bias_neuron()

mneurons = [0]*4
for i in range(4):
	mneurons[i] = sim.send_motor_neuron(joint_id=axles[i])
	sim.send_synapse(source_neuron_id=bias, target_neuron_id=mneurons[i], weight=1.0)

sim.film_body(body_id=box, method='follow')

sim.start()
sim.wait_to_finish()