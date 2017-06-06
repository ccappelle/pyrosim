import math
import sys, os
import numpy as np

import constants

from subprocess import Popen, PIPE

class Simulator:
	"""Python interface for ODE simulator
	
	Attributes
	----------
	play_blind   : bool, optional
		If True the simulation runs without graphics (headless) else if False the simulation 
		runs with graphics (the default is False)
	play_paused : bool, optional
		If True the simulation starts paused else if False the simulation starts running. 
		With simulation window in focus use Ctrl-p to toggle pausing the simulation. 
		(the default is False)
	eval_time    : int, optional
		The number of discrete steps in the simulation (the default is 100)
	dt 		    : float, optional
		The time in seconds between physics world steps. Larger dt values create more unstable
		physics. (the default is 0.05)
	gravity     : float, optional
		The gravity in the system. Negative values implie normal downward force of gravity.
		(default is -1.0)
	xyz		    : list of 3 floats
		The xyz position of the camera (default is [0.8317,-0.9817,0.8000])
	hpr
		The heading, pitch, and roll of the camera (default is [121,-27.5,0.0])
	use_textures: bool, optional
		Draw default ODE textures or not during simulation. (default is False)
	debug	    : bool, optional
		If True print out every string command sent through the pipe to the simulator
		(the default is False)
	"""
	def __init__(self,play_blind=False,play_paused=False,
					eval_time=constants.evaluation_time,dt=constants.dt, gravity=constants.gravity,
					xyz=constants.xyz, hpr=constants.hpr, use_textures=False,
					debug=False):

		self.numJoints = 0
		self.num_sensors = 0

		self.strings_to_send = []

		self.play_paused = play_paused
		self.play_blind = play_blind
		self.eval_time = eval_time
		self.dt = dt
		self.gravity = gravity
		self.debug = debug
		self.use_textures = use_textures
		self.evaluated = False

		self.pyrosim_path =  os.path.dirname(os.path.abspath(__file__))+'/simulator'

		if (self.play_paused == True and self.play_blind == True):
			self.play_paused = False

		#Initial simulator commands
		self._Send('TexturePath ' + self.pyrosim_path+'/textures'+'\n') 
		self._Send('EvaluationTime '+str(self.eval_time)+'\n')
		self._Send('TimeInterval ' + str(self.dt)+'\n')
		self._Send('Gravity ' + str(self.gravity)+'\n')
		self.Send_Camera(xyz, hpr)

	def Get_Sensor_Data(self,sensor_ID=0,svi=0):
		"""Get the post simulation data from a specified sensor

		Parameters
		----------
		sensor_ID : int , optional
			the sensors ID tag
		svi 	 : int , optional
			The sensor value index. Certain sensors have multiple values 
			(e.g. the position sensor) and the svi specifies which to 
			access (e.g. for a position sensor, svi=0 corresponds to the
			x value of that sensor)

		Returns
		-------
		list of float
			Returns the list of sensor values over the simulation.
		"""
		if self.evaluated:
			return self.data[sensor_ID,svi,:]
		else:
			return 'No Data'

	def Send_Bias_Neuron(self, neuron_ID = 0 ):
		"""Send bias neuron to simulator

		Parameters
		----------
		neuron_ID : int, optional
			User specified ID tag for the neuron

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'BiasNeuron'
		output_string = output_string + ' ' + str(neuron_ID)
		output_string = output_string + '\n'
		self._Send(output_string)

		return True

	def Send_Box(self, body_ID=0, x=0, y=0, z=0, length=0.1, width=0.1, height=0.1, r=1, g=1, b=1):
		"""Send box body to the simulator

		Parameters
		----------
		body_ID : int, optional
			User specified body ID tag for the box
		x 		 : float, optional
			The x position coordinate of the center 
		y		 : float, optional
			The y position coordinate of the center 
		z		 : float, optional
			The z position coordinate of the center 
		length   : float, optional
			The length of the box
		width   : float, optional
			The width of the box
		height  : float, optional
			The height of the box
		r       : float, optional
			The amount of the color red in the box (r in [0,1])
		g       : float, optional
			The amount of the color green in the box (g in [0,1])
		b       : float, optional
			The amount of the color blue in the box (b in [0,1])

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'Box'

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + ' ' + str(x)
		output_string = output_string + ' ' + str(y)
		output_string = output_string + ' ' + str(z)

		output_string = output_string + ' ' + str(length)
		output_string = output_string + ' ' + str(width)
		output_string = output_string + ' ' + str(height)

		output_string = output_string + ' ' + str(r)
		output_string = output_string + ' ' + str(g)
		output_string = output_string + ' ' + str(b)

		output_string = output_string + '\n'

		self._Send(output_string)
		return True

	def Send_Camera(self,xyz,hpr):
		"""Sends camera position to simulator in eulerian coordinates

		Parameters
		----------
		xyz : list of floats
			A length 3 list specifying the x,y,z position of the camera
			in simulation
		hpr : list of floats
			A length 3 list specifying the heading, pitch and roll of the camera

		Returns
		-------
		None
		"""
		
		output_string = 'Camera'
		self.xyz = xyz
		self.hpr = hpr

		for i in xyz:
			output_string = output_string + ' ' + str(i)
		for j in hpr:
			output_string = output_string + ' ' + str(j)

		output_string = output_string + '\n'
		self._Send(output_string)

	def Send_Cylinder(self, body_ID=0, x=0, y=0, z=0, r1=0, r2=0, r3=1, length=1.0, radius=0.1, r=1, g=1, b=1):
		"""Send cylinder body to the simulator

		Parameters
		----------
		body_ID : int, optional
			User specified body ID tag for the cylinder (default is 0)
		x 		 : float, optional
			The x position coordinate of the center (default is 0)
		y		 : float, optional
			The y position coordinate of the center (default is 0)
		z		 : float, optional
			The z position coordinate of the center (default is 0)
		r1		 : float, optional
			The orientation along the x axis. The vector [r1,r2,r3]
			specify the direction of the long axis of the cylinder.
			(default is 0)
		r2		 : float, optional
			The orientation along the y axis. The vector [r1,r2,r3]
			specify the direction of the long axis of the cylinder.
			(default is 0)
		r3		 : float, optional
			The orientation along the z axis. The vector [r1,r2,r3]
			specify the direction of the long axis of the cylinder.
			(default is 1)
		length   : float, optional
			The length of long axis of the cylinder (default is 1.0)
		radius   : float, optional
			The radius of the short axis of the cylinder (default is 0.1)
		r       : float, optional
			The amount of the color red in the box (r in [0,1])
		g       : float, optional
			The amount of the color green in the box (g in [0,1])
		b       : float, optional
			The amount of the color blue in the box (b in [0,1])

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'Cylinder'

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + ' ' + str(x)
		output_string = output_string + ' ' + str(y)
		output_string = output_string + ' ' + str(z)

		output_string = output_string + ' ' + str(r1)
		output_string = output_string + ' ' + str(r2)
		output_string = output_string + ' ' + str(r3)

		output_string = output_string + ' ' + str(length)
		output_string = output_string + ' ' + str(radius)

		output_string = output_string + ' ' + str(r)
		output_string = output_string + ' ' + str(g)
		output_string = output_string + ' ' + str(b)

		output_string = output_string + '\n'

		self._Send(output_string)
		return True

	def Send_User_Input_Neuron(self, neuron_ID=0, values=1):
		"""Send neuron to the simulator which takes user defined values at each time step
		
		Parameters
		----------
		neuron_ID : int, optional
			The user specified ID tag of the neuron
		values   : list of floats or float, optional
			The user specified values for the neuron. If length of values < the number of
			time steps, the values are continually looped through until every time step
			has a corresponding value

		Returns
		-------
		bool
			True if successful, False otherwise
		"""
		output_string = 'FunctionNeuron'

		try:
			iterator = iter(values)
		except TypeError:
			values = [values]*self.eval_time
		
		output_string = output_string + ' ' + str(neuron_ID)

		for i in range(self.eval_time):
			index = i%len(values)
			output_string = output_string + ' ' + str(values[index])

		output_string = output_string + '\n'
		self._Send(output_string)

		return True

	def Send_Function_Neuron(self, neuron_ID=0, function= math.sin):
		"""Send neuron to simulator which takes its value from the user defined function

		The function is mapped to the specific time in the simulation based on both 
		the discrete evaluation time and the dt space between time steps. For example
		if evalTime=100 and dt=0.05 the function will be evaluated at [0,0.05,...,5]

		Parameters
		----------
		neuron_ID : int, optional
			The user specified ID tag of the neuron
		function : function, optional
			The function which defines the neuron value. Valid functions return
			a single float value over the time domain.

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		end_time = self.eval_time*self.dt
		time_vals = np.arange(0,end_time,self.dt)
		output_vals = list(map(function,time_vals))
		return self.Send_User_Input_Neuron( neuron_ID, output_vals)

	def Send_Hidden_Neuron(self, neuron_ID = 0 , tau = 1.0 ):
		"""Send a hidden neuron to the simulator

		Hidden neurons are basic neurons which can have inputs and outputs. 
		They 'hidden' between input neurons (sensors, bias, function) and 
		output neurons (motors)

		Parameters
		----------
		neuron_ID : int, optional
			The user specified ID tag of the neuron
		tau      : float, optional
			The 'learning rate' of the neuron. Increasing tau increases
			how much of value of the neuron at the current time step comes
			from external inputs vs. the value of the neuron at the previous
			time step

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'HiddenNeuron'

		output_string = output_string + ' ' + str(neuron_ID)

		output_string = output_string + ' ' + str(tau)

		output_string = output_string + '\n'

		self._Send(output_string)

		return True

	def Send_Joint(self, joint_ID=0, first_body_ID=0, second_body_ID=1, x=0, y=0, z=0, n1=0, n2=0, n3=1, 
					lo=-math.pi/4.0, hi=+math.pi/4.0 , speed=1.0, torque=10.0, pos_control = True):
		"""Send a hinge joint to the simulator

		Parameters
		----------
		joint_ID 	    : int, optional
			User specified  ID tag for the joint (default is 0)
		first_body_ID   : int, optional
			The body ID of the first body the joint is connected to.
			If set equal to -1, the joint is connected to a point in
			space (default is 0)
		secondbodyID  : int, optional
			The body ID of the second body the joint is connected to.
			If set equal to -1, the joint is connected to a point in
			space (default is 1)
		x 		        : float, optional
			The x position coordinate of the joint (default is 0)
		y		 	    : float, optional
			The y position coordinate of the joint (default is 0)
		z		        : float, optional
			The z position coordinate of the joint (default is 0)
		n1		        : float, optional
			The orientation along the x axis. The vector [n1,n2,n3]
			specifies the axis about which the joint rotates
			(default is 0)
		n2		 	    : float, optional
			The orientation along the y axis. The vector [n1,n2,n3]
			specifies the axis about which the joint rotates
			(default is 0)
		n3		        : float, optional
			The orientation along the z axis. The vector [n1,n2,n3]
			specifies the axis about which the joint rotates
			(default is 1)
		lo              : float, optional
			The lower limit in radians of the joint (default is -pi/4)
		hi              : float, optional
			The upper limit in radians of the joint (default is pi/4)
		speed           : float, optional
			The speed of the motor of the joint (default is 1.0)
		pos_control : bool, optional
			True means use position control. This means the motor neuron
			output is treated as a target angle for the joint to actuate
			to. False means the motor neuron output is treated as a target
			actuation rate.

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'Joint'

		output_string = output_string + ' ' + str(joint_ID)

		output_string = output_string + ' ' + str(first_body_ID)
		output_string = output_string + ' ' + str(second_body_ID)

		output_string = output_string + ' ' + str(x)
		output_string = output_string + ' ' + str(y)
		output_string = output_string + ' ' + str(z)

		output_string = output_string + ' ' + str(n1)
		output_string = output_string + ' ' + str(n2)
		output_string = output_string + ' ' + str(n3)

		output_string = output_string + ' ' + str(lo)
		output_string = output_string + ' ' + str(hi)

		output_string = output_string + ' ' + str(speed)
		output_string = output_string + ' ' + str(torque)

		output_string = output_string + ' ' + str(pos_control)

		output_string = output_string + '\n'

		self._Send(output_string)

		return True

	def Send_Light_Sensor(self, sensor_ID=0, body_ID = 0 ):
		"""Attaches a light sensor to a body in simulation

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID of the sensor
		body_ID : int, optional
			The body ID of the body to connect the sensor to

		Returns
		-------
		bool
			True if successful
		"""

		output_string = 'LightSensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

		return True

	def Send_Light_Source(self, body_ID = 0 ):
		"""Attaches light source to a body in simulation

		Parameters
		----------
		body_ID : int, optional
			The body ID of the body to attach the light to

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'LightSource'

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		return True

	def Send_Motor_Neuron(self , neuron_ID = 0 , joint_ID = 0 , tau = 1.0 ):
		"""Send motor neurons to simulator

		Motor neurons are neurons which connecto to a specified joint and 
		determine how the joint moves every time step of simulation

		Parameters
		----------
		neuron_ID : int, optional
			The user specified ID tag of the neuron
		joint_ID  : int, optional
			The joint ID tag of the joint we want the neuron to connect to
		tau      :
			The 'learning rate' of the neuron. Increasing tau increases
			how much of value of the neuron at the current time step comes
			from external inputs vs. the value of the neuron at the previous
			time step

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'MotorNeuron'

		output_string = output_string + ' ' + str(neuron_ID)

		output_string = output_string + ' ' + str(joint_ID)

		output_string = output_string + ' ' + str(tau)

		output_string = output_string + '\n'

		self._Send(output_string)

	def Send_Position_Sensor(self, sensor_ID=0, body_ID = 0):
		"""Attaches a position sensor to a body in simulation

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID of the sensor
		body_ID : int, optional
			The body ID of the body to connect the sensor to

		Returns
		-------
		bool
			True if successful
		"""

		output_string = 'PositionSensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

	def Send_Proprioceptive_Sensor(self, sensor_ID=0, joint_ID = 0):
		"""Attaches a proprioceptive sensor to a joint in simulation

		Proprioceptive sensors returns the angle of the joint at 
		each time step

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID of the sensor
		joint_ID : int, optional
			The joint ID of the joint to connect the sensor to

		Returns
		-------
		bool
			True if successful
		"""

		output_string = 'ProprioceptiveSensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(joint_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

		return True

	def Send_Sensor_Neuron(self, neuron_ID=0, sensor_ID=0, svi=0, tau=1.0 ):
		"""Sends a sensor neuron to the simulator
	
		Sensor neurons are input neurons which take the value of their associated sensor

		Parameters
		----------
		neuron_ID 		 : int, optional
			The user defined ID of the neuron
		sensor_ID 		 : int, optional
			The associated sensor ID for the neuron
		svi : int, optional
			The sensor value index is the offset index of the sensor. SVI is used for 
			sensors which return a vector of values (position, ray sensors, etc.)
		tau   			 : int, optional
			not used for sensor neurons

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'SensorNeuron'

		output_string = output_string + ' ' + str(neuron_ID)

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(svi)

		output_string = output_string + ' ' + str(tau)

		output_string = output_string + '\n'

		self._Send(output_string)

	def Send_Ray_Sensor(self, sensor_ID=0, body_ID=0, x=0,y=0,z=0, r1=0,r2=0,r3=1):
		"""Sends a ray sensor to the simulator connected to a body

		Ray sensors return four values each time step, the distance and color (r,g,b).

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID tag for the ray sensor
		body_ID : int, optional
			The body ID of the associated body the ray sensor is connected to. When this
			body moves the ray sensor moves accordingly
		x 		 : float, optional
			The x position of the sensor
		y		 : float, optional
			The y position of the sensor
		z 		 : float, optional
			The z position of the sensor
		r1       : float, optional
			The x direction of the sensor. The array [r1,r2,r3] is the direction the
			ray sensor is pointing in the time step.
		r2       : float, optional
			The y direction of the sensor. The array [r1,r2,r3] is the direction the
			ray sensor is pointing in the time step.
		r3       : float, optional
			The z direction of the sensor. The array [r1,r2,r3] is the direction the
			ray sensor is pointing in the time step.
		"""

		output_string = 'RaySensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + ' ' + str(x)
		output_string = output_string + ' ' + str(y)
		output_string = output_string + ' ' + str(z)

		output_string = output_string + ' ' + str(r1)
		output_string = output_string + ' ' + str(r2)
		output_string = output_string + ' ' + str(r3)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

		return True

	def Send_Synapse(self, source_neuron_ID = 0 , target_neuron_ID = 0 , weight = 0.0 ):
		"""Sends a synapse to the simulator

		Synapses are the edge connections between neurons

		Parameters
		----------
		source_neuron_ID : int, optional
			The ID of the source neuron of the synapse
		target_neuron_ID : int, optional
			The ID of the target neuron of the synapse
		weight		   : float, optional
			The edge weight of the synapse

		Returns:
		--------
		bool
			True if successful, False otherwise
		"""


		return self.Send_Developing_Synapse(source_neuron_ID=sourceneuron_ID, target_neuron_ID=targetneuron_ID,
										start_weight=weight,end_weight=weight,
										start_time=0.,end_time=0.)

	def Send_Developing_Synapse(self, source_neuron_ID=0, target_neuron_ID=0, 
								start_weight=0.0, end_weight=0.0, 
								start_time=0., end_time=1.0):
		"""Sends a synapse to the simulator

		Developing synapses are synapses which change over time. 
		The synapse will interpolate between the start_weight and end_weight
		over the desired time range dictated by start_time and end_time.
		start_time and end_time are in [0,1] where 0 maps to time step 0
		and 1 maps to the eval_time of the simulation. Setting start_time
		equal to end_time results in a discrete change from start_weight
		to end_weight in the synapse at the specified time step. If
		start_time >= end_time times are changed such that
		end_time = start_time.

		Parameters
		----------
		source_neuron_ID : int, optional
			The ID of the source neuron of the synapse
		target_neuron_ID : int, optional
			The ID of the target neuron of the synapse
		start_weight	   : float, optional
			The starting edge weight of the synapse
		end_weight 	   : float, optional
			The ending edge weight of the synapse
		start_time	   : float, optional
			The starting time of development. start_time in [0,1]
		end_time 	   : float, optional
			The ending time of development. end_time in [0,1]
		Returns:
		--------
		bool
			True if successful, False otherwise
		"""
		if start_time >= end_time:
			end_time = start_time

		start_time = int(start_time * (self.eval_time-1))
		end_time = int(end_time * (self.eval_time-1))

		output_string = 'Synapse'

		output_string = output_string + ' ' + str(source_neuron_ID)

		output_string = output_string + ' ' + str(target_neuron_ID)

		output_string = output_string + ' ' + str(start_weight)

		output_string = output_string + ' ' + str(end_weight)

		output_string = output_string + ' ' + str(start_time)

		output_string = output_string + ' ' + str(end_time)		

		output_string = output_string + '\n'

		self._Send(output_string)

		return True

	def Send_Touch_Sensor(self, sensor_ID=0, body_ID=0):
		"""Send touch sensor to a body in the simulator

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID of the sensor
		body_ID : int, optional
			The body ID of the associated body 

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'TouchSensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

		return True

	def Send_Vestibular_Sensor(self, sensor_ID=0, body_ID = 0):
		"""Connects a vestibular sensor to a body

		Vestibular sensors return a bodies orrientation in space

		Parameters
		----------
		sensor_ID : int, optional
			The user defined ID of the sensor
		body_ID : int, optional
			The body ID of the associated body 

		Returns
		-------
		bool
			True if successful, False otherwise
		"""

		output_string = 'VestibularSensor'

		output_string = output_string + ' ' + str(sensor_ID)

		output_string = output_string + ' ' + str(body_ID)

		output_string = output_string + '\n'

		self._Send(output_string)

		self.num_sensors = self.num_sensors + 1

		return True

	def Start(self):
		"""Starts the simulation"""

		commandsToSend = [self.pyrosim_path + '/simulator']

		if ( self.play_blind == True ):

			commandsToSend.append('-blind')
		else:
			if self.use_textures == False:
				commandsToSend.append('-notex')

		if ( self.play_paused == True ):
			commandsToSend.append('-pause')

		self.pipe = Popen(commandsToSend, stdout=PIPE, stdin=PIPE, stderr=PIPE)

		#self.Send('Done\n')
		for string_to_send in self.strings_to_send:
			self.pipe.stdin.write(string_to_send)

		self.pipe.stdin.write('Done\n')

		return True

	def Wait_To_Finish(self):
		"""Waits to for the simulation to finish and collects data

		Returns
		-------
		numpy matrix
			A matrix of the sensor values for each time step of the simulation
		"""

		data_from_simulator = self.pipe.communicate()

		self._Collect_Sensor_Data(data_from_simulator)
		self.evaluated = True

		return self.data

# --------------------- Private methods -----------------------------

	def _Collect_Sensor_Data(self,data_from_simulator):

		self.data = np.zeros([self.num_sensors,4,self.eval_time],dtype='f')

		if self.debug:
		 	print data_from_simulator[1]

		data_from_simulator = data_from_simulator[0]

		data_from_simulator = data_from_simulator.split()

		index = 0

		if ( data_from_simulator == [] ):
			return

		while ( data_from_simulator[index] != 'Done' ):
			ID = int( data_from_simulator[index] )
			index = index + 1

			num_sensor_vals = int( data_from_simulator[index] ) 

			index = index + 1

			for t in range(0,self.eval_time):

				for s in range(0,num_sensor_vals):

					sensorValue = float( data_from_simulator[index] )

					self.data[ID,s,t] = sensorValue

					index = index + 1

	def _Send(self,string_to_send):
		if self.debug:
			print string_to_send
		self.strings_to_send.append(string_to_send)
		#self.simulator.stdin.write( stringToSend )
