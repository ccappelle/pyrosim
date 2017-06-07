import pyrosim

WHEEL_RADIUS = 0.1

sim = pyrosim.Simulator(play_paused=False, eval_time=1000)

wheel = sim.send_cylinder(x=-2*WHEEL_RADIUS, y=-2*WHEEL_RADIUS, z=WHEEL_RADIUS, length=0, radius=WHEEL_RADIUS)

box = sim.send_box(x=0,y=0,z=WHEEL_RADIUS, length=4*WHEEL_RADIUS, width=4*WHEEL_RADIUS, height=WHEEL_RADIUS)

#position_control = False -> continuous range of motion
axle = sim.send_hinge_joint(first_body_id=wheel, second_body_id=box, x=-2*WHEEL_RADIUS, 
						y=-2*WHEEL_RADIUS, z=WHEEL_RADIUS, n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0, 
						position_control = False)

touch = sim.send_touch_sensor(body_id=wheel)

sneuron = sim.send_sensor_neuron(sensor_id=touch)

mneuron = sim.send_motor_neuron(joint_id=axle)

sim.send_synapse(source_neuron_id=sneuron, target_neuron_id=mneuron, weight=1.0)

sim.start()