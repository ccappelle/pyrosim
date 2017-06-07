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

	main_body = sim.Send_Box(x=0, y=0,z=HEIGHT+EPS,
				 length=HEIGHT, width=HEIGHT, height=EPS*2.0)

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

		thighs[i] = sim.Send_Cylinder( x=x_pos, y=y_pos, z=HEIGHT+EPS,
							r1=x_pos, r2=y_pos, r3=0,
							length=HEIGHT,radius=EPS
							)

		hips[i] = sim.Send_Hinge_Joint(first_body_ID=main_body, second_body_ID=thighs[i],
						x=x_pos/2.0, y=y_pos/2.0, z=HEIGHT+EPS,
						n1=-y_pos,n2=x_pos,n3=0,
						lo=-math.pi/4.0, hi=math.pi/4.0,
						speed=1.0)

		motor_neurons[i] = sim.Send_Motor_Neuron( joint_ID=hips[i])

		x_pos2 = math.cos(theta)*1.5*HEIGHT
		y_pos2 = math.sin(theta)*1.5*HEIGHT
		
		shins[i] = sim.Send_Cylinder(x=x_pos2, y=y_pos2, z=(HEIGHT+EPS)/2.0,
							r1=0, r2=0, r3=1,
							length=HEIGHT, radius=EPS)

		knees[i] = sim.Send_Hinge_Joint( first_body_ID=thighs[i], second_body_ID=shins[i],
						x=x_pos2, y=y_pos2, z=HEIGHT+EPS,
						n1=-y_pos,n2=x_pos,n3=0,
						lo=-math.pi/4.0,hi=math.pi/4.0)

		motor_neurons[i+4] = sim.Send_Motor_Neuron( joint_ID=knees[i])

		foot_sensors[i] = sim.Send_Touch_Sensor( body_ID=shins[i])

		sensor_neurons[i] = sim.Send_Sensor_Neuron( sensor_ID=foot_sensors[i])


	light_sensor = sim.Send_Light_Sensor(body_ID=main_body)

	sensor_neurons[-1] = sim.Send_Sensor_Neuron( sensor_ID=light_sensor)

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
