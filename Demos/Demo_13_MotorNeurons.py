import pyrosim

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = pyrosim.Simulator(play_paused=False, eval_time=1000)

cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS, r1=0, r2=0, r3=1, 
					length=ARM_LENGTH, radius=ARM_RADIUS)

cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS, 
					r1=0, r2=1, r3=0, length=ARM_LENGTH, radius=ARM_RADIUS)

joint = sim.send_hinge_joint(first_body_id=cyl1, second_body_id=cyl2, x=0, y=0, z=ARM_LENGTH + ARM_RADIUS, 
				n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

touch1 = sim.send_touch_sensor(body_id = cyl1)
touch2 = sim.send_touch_sensor(body_id = cyl2)

#Again no synapses so no movement
sensor_neuron1 = sim.send_sensor_neuron(sensor_id=touch1)
sensor_neuron2 = sim.send_sensor_neuron(sensor_id=touch2)
#the motor neuron will output a constant 0 to the joint
#so the joint will not rotate
motor_neuron = sim.send_motor_neuron(joint_id=joint)

sim.start()

# sim.Collect_Sensor_Data()
