import pyrosim

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = pyrosim.Simulator(play_paused = False , eval_time = 1000)

cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS, r1=0, r2=0, r3=1, 
					length=ARM_LENGTH, radius=ARM_RADIUS)

cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS, r1=0, r2=1, r3=0, 
							length=ARM_LENGTH, radius=ARM_RADIUS)

sim.send_hinge_joint(first_body_id=cyl1, second_body_id=cyl2, x=0, y=0, z=ARM_LENGTH + ARM_RADIUS, 
					n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

#Touch sensors return 1 or 0 depending on touching or not
#stored at svi=0
sensor1 = sim.send_touch_sensor(body_id=cyl1)
sensor2 = sim.send_touch_sensor(body_id=cyl2)

sim.start()

#results is a 3D numpy matrix containing sensor data
results = sim.wait_to_finish()
print results

#you can also use built in functions to find values
#of specific sensors over the evaluation time
print sim.get_sensor_data(sensor1, svi=0)
print sim.get_sensor_data(sensor2, svi=0)