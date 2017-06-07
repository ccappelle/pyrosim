import math
import numpy as np

HEIGHT = 0.3
EPS = 0.05

def Send_To_Simulator(sim, weight_matrix):
	#if np.shape(weight_matrix) != ()
	body_ID = 0
	joint_ID = 0
	sensor_ID = 0
	neuron_ID = 0

	main_body = body_ID
	sim.Send_Box(body_ID=body_ID, 
				 x=0, y=0,z=HEIGHT+EPS,
				 length=HEIGHT, width=HEIGHT, height=EPS*2.0)
	body_ID += 1

	thighs = [0]*4
	shins = [0]*4
	hips = [0]*4
	knees = [0]*4
	foot_sensors = [0]*4
	sensor_neurons = [0]*5
	motor_neurons = [0]*8

	delta = float(math.pi)/2.0

	for i in range(4):
		theta = delta*i
		x_pos = math.cos(theta)*HEIGHT
		y_pos = math.sin(theta)*HEIGHT

		thighs[i] = body_ID
		sim.Send_Cylinder(body_ID=body_ID, 
							x=x_pos, y=y_pos, z=HEIGHT+EPS,
							r1=x_pos, r2=y_pos, r3=0,
							length=HEIGHT,radius=EPS
							)
		body_ID+=1

		hips[i] = joint_ID
		sim.Send_Hinge_Joint(joint_ID=joint_ID,
						first_body_ID=main_body, second_body_ID=thighs[i],
						x=x_pos/2.0, y=y_pos/2.0, z=HEIGHT+EPS,
						n1=-y_pos,n2=x_pos,n3=0,
						lo=-math.pi/4.0, hi=math.pi/4.0)
		joint_ID+=1

		motor_neurons[i] = neuron_ID
		sim.Send_Motor_Neuron(neuron_ID=neuron_ID, joint_ID=hips[i])
		neuron_ID += 1

		x_pos2 = math.cos(theta)*1.5*HEIGHT
		y_pos2 = math.sin(theta)*1.5*HEIGHT
		
		shins[i] = body_ID
		sim.Send_Cylinder(body_ID=body_ID,
							x=x_pos2, y=y_pos2, z=(HEIGHT+EPS)/2.0,
							r1=0, r2=0, r3=1,
							length=HEIGHT, radius=EPS)
		body_ID+=1

		knees[i] = joint_ID
		sim.Send_Hinge_Joint(joint_ID=joint_ID,
						first_body_ID=thighs[i], second_body_ID=shins[i],
						x=x_pos2, y=y_pos2, z=HEIGHT+EPS,
						n1=-y_pos,n2=x_pos,n3=0,
						lo=-math.pi/4.0,hi=math.pi/4.0)
		joint_ID+=1

		motor_neurons[i+4] = neuron_ID
		sim.Send_Motor_Neuron(neuron_ID=neuron_ID, joint_ID=knees[i])
		neuron_ID += 1

		foot_sensors[i] = sensor_ID
		sim.Send_Touch_Sensor(sensor_ID=sensor_ID, body_ID=shins[i])
		sensor_ID += 1

		sensor_neurons[i] = neuron_ID
		sim.Send_Sensor_Neuron(neuron_ID=neuron_ID, sensor_ID=foot_sensors[i])
		neuron_ID += 1


	light_sensor = sensor_ID
	sim.Send_Light_Sensor(sensor_ID=light_sensor, body_ID=main_body)
	sensor_ID+=1

	sensor_neurons[-1] = neuron_ID
	sim.Send_Sensor_Neuron(neuron_ID=neuron_ID, sensor_ID=light_sensor)
	neuron_ID += 1

	count = 0
	#sim.Send_Synapse(sourceneuron_ID=2, targetneuron_ID=0, weight=1.0)
	for source_ID in sensor_neurons:
		for target_ID in motor_neurons:
			count += 1
			start_weight = weight_matrix[source_ID,target_ID,0]
			end_weight = weight_matrix[source_ID, target_ID,1]
			time = weight_matrix[source_ID, target_ID,2]
			#time = .5
			sim.Send_Developing_Synapse(source_neuron_ID=source_ID, target_neuron_ID=target_ID,
										start_weight=start_weight, end_weight = end_weight,
										start_time=time,end_time=time)

	layout = {'thighs': thighs,
				'shins': shins,
				'hips': hips,
				'knees': knees,
				'feet': foot_sensors,
				'light': light_sensor,
				'sensor_neurons': sensor_neurons,
				'motor_neurons': motor_neurons}

	return layout

if __name__=="__main__":
	import pyrosim

	eval_time = 100
	gravity = -1.0

	sim = pyrosim.Simulator(eval_time=eval_time, debug=True, gravity=gravity,play_blind=False, use_textures=True)
	num_sensors = 5
	num_motors = 8

	weight_matrix = np.random.rand(num_sensors+num_motors,num_sensors+num_motors,3)
	weight_matrix[:,:,0:1] = weight_matrix[:,:,0:1]*2.-1.
	time_matrix = np.random.rand(num_sensors+num_motors)

	layout = Send_To_Simulator(sim, weight_matrix=weight_matrix)
	sim.Start()
	results = sim.Wait_To_Finish()
