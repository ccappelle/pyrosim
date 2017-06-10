import math
import sys, os
import numpy as np

import constants

from subprocess import Popen, PIPE

class Simulator(object):
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
	use_textures : bool, optional
		Draw default ODE textures or not during simulation. (default is False)
	debug	    : bool, optional
		If True print out every string command sent through the pipe to the simulator
		(the default is False)
	"""
	def __init__(self,play_blind=False,play_paused=False,
					eval_time=constants.evaluation_time,dt=constants.dt, gravity=constants.gravity,
					xyz=constants.xyz, hpr=constants.hpr, use_textures=False,
					debug=False):
		self.strings_to_send = []

		self._num_bodies = 0
		self._num_joints = 0
		self._num_sensors = 0
		self._num_neurons = 0

		self.play_paused = play_paused
		self.play_blind = play_blind
		self.eval_time = eval_time
		self.dt = dt
		self.gravity = gravity
		self.debug = debug
		self.use_textures = use_textures
		self.evaluated = False

		self.pyrosim_path =  os.path.dirname(os.path.abspath(__file__))+'/simulator'

		self.body_to_follow= -1

		if debug:
			print self.pyrosim_path

		if (self.play_paused == True and self.play_blind == True):
			self.play_paused = False

		#Initial simulator commands
		self._send('TexturePath ' + self.pyrosim_path+'/textures') 
		self._send('EvaluationTime '+str(self.eval_time))
		self._send('TimeInterval ' + str(self.dt))
		self._send('Gravity ' + str(self.gravity))
		if (self.debug):
			self._send('Debug '+str(1))
		else:
			self._send('Debug '+str(0))
		self.send_camera(xyz, hpr)

	def film_body(self, body_id, method='follow'):
		"""Sets the camera to film a body
		
		Camera has two modes: 'follow' moves the camera's position based on where the body is moving
		and 'track' rotates the camera to look at the body

		Parameters
		----------
		body_id : int
			The id tag of the body to be filmed
		method  : str, optional
			The way the camera should move to film the body. Either 'follow' or 'track' 
			(default is 'follow')

		Returns
		-------
		bool
			True if successful, False otherwise
		"""
		assert body_id<self._num_bodies, 'Body with id ' + str(body_id) + ' has not been sent'
		assert self.body_to_follow==-1, 'Body with id ' + str(body_id) +' has already been assigned to be filmed'
		assert (method=='follow' or method=='track'), 'Method must be `follow` or `track`'

		if method == 'follow':
			self._send('FollowBody', body_id)
		elif method == 'track':
			self._send('TrackBody', body_id)

		self.body_to_follow = body_id

		return True

	def get_data(self):
		"""Get all sensor data back as numpy matrix"""
		assert self.evaluated == True, 'Simulation has not run yet'
		return self.data

	def get_num_bodies(self):
		"""Returns the number of bodies"""
		return self._num_bodies

	def get_num_joints(self):
		"""Returns the number of joints"""
		return self._num_joints

	def get_num_neurons(self):
		"""Returns the number of neurons"""
		return self._num_neurons
		
	def get_num_sensors(self):
		"""Returns the number of sensors"""
		return self._num_sensors

	def get_sensor_data(self,sensor_id,svi=0):
		"""Get the post simulation data from a specified sensor

		Parameters
		----------
		sensor_id : int
			the sensors id tag
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
		assert self.evaluated==True, 'Simulation has not run yet'
		return self.data[sensor_id,svi,:]

	def send_bias_neuron(self):
		"""Send bias neuron to simulator.

		Bias neurons emit a constant value of 1.0

		Returns
		-------
		int
			id tag of the neuron
		"""
		neuron_id = self._num_neurons
		self._num_neurons += 1

		self._send('BiasNeuron',
					neuron_id)

		return neuron_id

	def send_box(self, x=0, y=0, z=0, length=0.1, width=0.1, height=0.1, r=1, g=1, b=1):
		"""Send box body to the simulator

		Parameters
		----------
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
		int
			id tag of the box
		"""
		assert length>0, 'Length of Box must be positive'
		assert width>0 , 'Width of Box must be positive'
		assert height>0, 'Height of Box must be positive'
		self._assert_color( 'Box', r, g,b)

		body_id = self._num_bodies
		self._num_bodies += 1

		self._send('Box', 
					body_id, 
					x,y,z,
					length,width,height,
					r,g,b)

		return body_id

	def send_camera(self,xyz,hpr):
		"""Sends camera position to simulator in eulerian coordinates

		Parameters
		----------
		xyz : list of floats
			A length 3 list specifying the x,y,z position of the camera
			in simulation
		hpr : list of floats
			A length 3 list specifying the heading, pitch, and roll of the camera

		Returns
		-------
		bool
			True if successful, False otherwise
		"""	
		self._send('Camera',
					xyz[0],xyz[1],xyz[2],
					hpr[0],hpr[1],hpr[2])

		return True

	def send_cylinder(self, x=0, y=0, z=0, r1=0, r2=0, r3=1, length=1.0, radius=0.1, r=1, g=1, b=1):
		"""Send cylinder body to the simulator

		Parameters
		----------
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
		int
			The id tag of the cylinder
		"""
		assert length>=0, 'Length of Cylinder must be >= 0'
		assert radius>=0, 'Radius of Cylinder must be >= 0'
		self._assert_normalizable('Cylinder',r1,r2,r3)
		self._assert_color('Cylinder',r,g,b)

		body_id = self._num_bodies
		self._num_bodies += 1

		self._send('Cylinder',
					body_id,
					x,y,z,
					r1,r2,r3,
					length,radius,
					r,g,b)

		return body_id

	def send_user_input_neuron(self, in_values):
		"""Send neuron to the simulator which takes user defined values at each time step
		
		Parameters
		----------
		in_values : list of floats or float, optional
			The user specified values for the neuron. If length of values < the number of
			time steps, the values are continually looped through until every time step
			has a corresponding value

		Returns
		-------
		int
			The id tag of the neuron.
		"""

		try:
			iterator = iter(in_values)
		except TypeError:
			in_values = [in_values]*self.eval_time

		#if values is shorter than eval_time repeat until sufficient
		if len(in_values)<self.eval_time:
			out_values = [0]*self.eval_time
			for i in range(self.eval_time):
				out_values[i] = in_values[i%len(in_values)]
		else:
			out_values = in_values

		neuron_id = self._num_neurons
		self._num_neurons += 1


		self._send('FunctionNeuron', neuron_id, *out_values)

		return neuron_id

	def send_function_neuron(self, function= math.sin):
		"""Send neuron to simulator which takes its value from the user defined function

		The function is mapped to the specific time in the simulation based on both 
		the discrete evaluation time and the dt space between time steps. For example
		if evalTime=100 and dt=0.05 the function will be evaluated at [0,0.05,...,5]

		Parameters
		----------
		function : function, optional
			The function which defines the neuron value. Valid functions return
			a single float value over the time domain.

		Returns
		-------
		int
			The id tag of the neuron
		"""
		end_time = self.eval_time*self.dt
		time_vals = np.arange(0,end_time,self.dt)
		output_vals = list(map(function,time_vals))

		return self.send_user_input_neuron(output_vals)

	def send_hidden_neuron(self, tau=1.0):
		"""Send a hidden neuron to the simulator

		Hidden neurons are basic neurons which can have inputs and outputs. 
		They 'hidden' between input neurons (sensors, bias, function) and 
		output neurons (motors)

		Parameters
		----------
		tau      : float, optional
			The 'learning rate' of the neuron. Increasing tau increases
			how much of value of the neuron at the current time step comes
			from external inputs vs. the value of the neuron at the previous
			time step

		Returns
		-------
		int
			The id tag of the neuron
		"""
		assert tau>0, 'Tau value of Hidden Neuron must be positive'
		neuron_id = self._num_neurons
		self._num_neurons += 1	

		self._send('HiddenNeuron',
					neuron_id, tau)

		return neuron_id

	def send_hinge_joint(self, first_body_id, second_body_id, x=0, y=0, z=0, n1=0, n2=0, n3=1, 
					lo=-math.pi/4.0, hi=+math.pi/4.0 , speed=1.0, torque=10.0, position_control = True):
		"""Send a hinge joint to the simulator

		Parameters
		----------
		first_body_id   : int
			The body id of the first body the joint is connected to.
			If set equal to -1, the joint is connected to a point in
			space 
		secondbodyid  : int
			The body id of the second body the joint is connected to.
			If set equal to -1, the joint is connected to a point in
			space 
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
		torque 			: float, optional
			The amount of torque the motor in the joint has (default is 10.0)
		position_control : bool, optional
			True means use position control. This means the motor neuron
			output is treated as a target angle for the joint to actuate
			to. False means the motor neuron output is treated as a target
			actuation rate.

		Returns
		-------
		int 
			The id tag for the hinge joint
		"""
		assert first_body_id<self._num_bodies, 'Body with id ' + str(first_body_id)+ ' has not been sent'
		assert second_body_id<self._num_bodies, 'Body with id ' + str(first_body_id)+ ' has not been sent'
		assert speed>=0, 'Speed of Hinge Joint must be greater than or equal to zero'
		assert torque>=0, 'Torque of Hinge Joint must be greater than or equal to zero'
		self._assert_normalizable('Hinge Joint',n1,n2,n3)

		joint_id = self._num_joints
		self._num_joints += 1

		self._send('HingeJoint',
					joint_id, 
					first_body_id, second_body_id,
					x, y, z,
					n1, n2, n3,
					lo, hi,
					speed, torque,
					position_control)

		return joint_id

	def send_light_sensor(self, body_id = 0 ):
		"""Attaches a light sensor to a body in simulation

		Parameters
		----------
		body_id : int, optional
			The body id of the body to connect the sensor to

		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert body_id<self._num_bodies, 'Body id '+str(body_id)+' has not been sent'

		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('LightSensor',
					sensor_id, body_id)

		return sensor_id

	def send_light_source(self, body_id=0):
		"""Attaches light source to a body in simulation

		Parameters
		----------
		body_id : int, optional
			The body id of the body to attach the light to

		Returns
		-------
		int
			The id tag of the body the light source is attached to.
		"""
		assert body_id<self._num_bodies, 'Body id '+str(body_id)+' has not been sent'

		self._send('LightSource',
					body_id)

		return body_id

	def send_motor_neuron(self , joint_id=0, tau=1.0):
		"""Send motor neurons to simulator

		Motor neurons are neurons which connecto to a specified joint and 
		determine how the joint moves every time step of simulation

		Parameters
		----------
		joint_id  : int, optional
			The joint id tag of the joint we want the neuron to connect to
		tau      :
			The 'learning rate' of the neuron. Increasing tau increases
			how much of value of the neuron at the current time step comes
			from external inputs vs. the value of the neuron at the previous
			time step

		Returns
		-------
		int
			The id tag of the neuron
		"""
		assert tau>=0, 'Tau must be positive'
		assert joint_id<self._num_joints, 'Joint with id '+str(joint_id)+' has not been sent'

		neuron_id = self._num_neurons
		self._num_neurons += 1

		self._send('MotorNeuron',
					neuron_id,  joint_id, tau)

		return neuron_id

	def send_position_sensor(self, body_id=0):
		"""Attaches a position sensor to a body in simulation

		Parameters
		----------
		body_id : int, optional
			The body id of the body to connect the sensor to

		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert body_id<self._num_bodies, 'Body with id '+str(body_id)+' has not been sent'

		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('PositionSensor',
					sensor_id, body_id)

		return sensor_id

	def send_proprioceptive_sensor(self, joint_id=0):
		"""Attaches a proprioceptive sensor to a joint in simulation

		Proprioceptive sensors returns the angle of the joint at 
		each time step

		Parameters
		----------
		joint_id : int, optional
			The joint id of the joint to connect the sensor to

		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert joint_id<self._num_joints, 'Joint with id '+str(joint_id)+' has not been sent'

		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('ProprioceptiveSensor',
					sensor_id, joint_id)

		return sensor_id

	def send_sensor_neuron(self, sensor_id=0, svi=0, tau=1.0):
		"""Sends a sensor neuron to the simulator
	
		Sensor neurons are input neurons which take the value of their associated sensor

		Parameters
		----------
		sensor_id 		 : int, optional
			The associated sensor id for the neuron to draw values from.
		svi : int, optional
			The sensor value index is the offset index of the sensor. SVI is used for 
			sensors which return a vector of values (position, ray sensors, etc.)
		tau   			 : int, optional
			not used for sensor neurons

		Returns
		-------
		int
			The id tag of the neuron
		"""
		assert sensor_id<self._num_sensors, 'Sensor with id '+str(sensor_id)+' has not been sent'
		assert svi in range(4), 'SVI must be in [0,3]'
		assert tau>=0, 'Tau must be greater than or equal to zero'

		neuron_id = self._num_neurons
		self._num_neurons += 1

		self._send('SensorNeuron',
					neuron_id, sensor_id,
					svi, tau)

		return neuron_id

	def send_sphere(self, x=0,y=0,z=0,radius=0.5, r=1,g=1,b=1):
		"""Sends a sphere to the simulator

		Parameters
		----------
		x 		 : float, optional
			The x position of the center
		y		 : float, optional
			The y position of the center
		z 		 : float, optional
			The z position of the center
		radius   : float, optional
			The radius of the sphere (default is 0.5)
		r       : float, optional
			The amount of the color red in the box (r in [0,1])
		g       : float, optional
			The amount of the color green in the box (g in [0,1])
		b       : float, optional
			The amount of the color blue in the box (b in [0,1])

		Returns
		-------
		int     
			The id tag of the sphere
		"""
		assert radius>=0, 'Radius of Sphere must be >= 0'
		self._assert_color( 'Sphere', r, g,b)

		body_id = self._num_bodies
		self._num_bodies += 1

		self._send('Sphere',
					body_id, 
					x,y,z,
					radius,
					r,g,b)

		return body_id

	def send_ray_sensor(self, body_id=0, x=0,y=0,z=0, r1=0,r2=0,r3=1):
		"""Sends a ray sensor to the simulator connected to a body

		Ray sensors return four values each time step, the distance and color (r,g,b).

		Parameters
		----------
		body_id : int, optional
			The body id of the associated body the ray sensor is connected to. When this
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
		
		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert body_id<self._num_bodies, 'Body with id ' + str(body_id) + ' has not been sent yet'
		self._assert_normalizable('Ray Sensor', r1,r2,r3)

		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('RaySensor',
					sensor_id, body_id,
					x,y,z,
					r1,r2,r3)

		return sensor_id

	def send_synapse(self, source_neuron_id = 0 , target_neuron_id = 0 , weight = 0.0 ):
		"""Sends a synapse to the simulator

		Synapses are the edge connections between neurons

		Parameters
		----------
		source_neuron_id : int, optional
			The id of the source neuron of the synapse
		target_neuron_id : int, optional
			The id of the target neuron of the synapse
		weight		   : float, optional
			The edge weight of the synapse

		Returns
		-------
		bool
			True if successful, False otherwise
		"""
		assert source_neuron_id<self._num_neurons, 'Neuron with id '+str(source_neuron_id)+' has not been sent'
		assert target_neuron_id<self._num_neurons, 'Neuron with id '+str(target_neuron_id)+' has not been sent'

		return self.send_developing_synapse(source_neuron_id=source_neuron_id, target_neuron_id=target_neuron_id,
										start_weight=weight,end_weight=weight,
										start_time=0.,end_time=0.)

	def send_developing_synapse(self, source_neuron_id=0, target_neuron_id=0, 
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
		source_neuron_id : int, optional
			The id of the source neuron of the synapse
		target_neuron_id : int, optional
			The id of the target neuron of the synapse
		start_weight	   : float, optional
			The starting edge weight of the synapse
		end_weight 	   : float, optional
			The ending edge weight of the synapse
		start_time	   : float, optional
			The starting time of development. start_time in [0,1]
		end_time 	   : float, optional
			The ending time of development. end_time in [0,1]
		Returns
		-------
		bool
			True if successful, False otherwise
		"""
		assert source_neuron_id<self._num_neurons, 'Neuron with id '+str(source_neuron_id)+' has not been sent'
		assert target_neuron_id<self._num_neurons, 'Neuron with id '+str(target_neuron_id)+' has not been sent'

		if start_time >= end_time:
			end_time = start_time

		start_time = int(start_time * (self.eval_time-1))
		end_time = int(end_time * (self.eval_time-1))

		self._send('Synapse',
					source_neuron_id, target_neuron_id,
					start_weight, end_weight,
					start_time, end_time)

		return True

	def send_touch_sensor(self, body_id=0):
		"""Send touch sensor to a body in the simulator

		Parameters
		----------
		body_id : int, optional
			The body id of the associated body 

		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert body_id<self._num_bodies, 'Body with id '+body_id+' has not been sent yet'
		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('TouchSensor',
					sensor_id, body_id)

		return sensor_id

	def send_vestibular_sensor(self, body_id = 0):
		"""Connects a vestibular sensor to a body

		Vestibular sensors return a bodies orrientation in space

		Parameters
		----------
		body_id : int, optional
			The body id of the associated body 

		Returns
		-------
		int
			The id tag of the sensor
		"""
		assert body_id<self._num_bodies, 'Body with id '+body_id+' has not been sent yet'

		sensor_id = self._num_sensors
		self._num_sensors += 1

		self._send('VestibularSensor', 
					sensor_id, body_id)

		return sensor_id

	def start(self):
		"""Starts the simulation"""

		commands = [self.pyrosim_path + '/simulator']

		if ( self.play_blind == True ):
			commands.append('-blind')
		else:
			if self.use_textures == False:
				commands.append('-notex')

		if ( self.play_paused == True ):
			commands.append('-pause')

		self.pipe = Popen(commands, stdout=PIPE, stdin=PIPE, stderr=PIPE)

		for string_to_send in self.strings_to_send:
			self.pipe.stdin.write(string_to_send)

		self.pipe.stdin.write('Done\n')
		#self.pipe.stdin.write('textuer2\n')
		return True

	def wait_to_finish(self):
		"""Waits to for the simulation to finish and collects data

		Returns
		-------
		numpy matrix
			A matrix of the sensor values for each time step of the simulation
		"""

		data_from_simulator = self.pipe.communicate()

		self._collect_sensor_data(data_from_simulator)
		self.evaluated = True

		return self.data

# --------------------- Private methods -----------------------------
	def _assert_color(self,name,r,g,b):
		"""Error checks so color params are between [0,1]"""
		colors = [r,g,b]
		for color in colors:
			assert color>=0 and color<=1, 'Color parameter of ' + name +' must be in [0,1]'
	def _assert_normalizable(self,name,*args):
		"""Error checxs vectors so they are not equal to zero"""
		flag = False
		for arg in args:
			if arg != 0:
				flag = True
				
		assert flag==True, 'Vector parameter of ' + name + ' is not normalizeable'

	def _collect_sensor_data(self,data_from_simulator):
		"""Get sensor data back from ODE and store it in numpy array"""

		self.data = np.zeros([self._num_sensors,4,self.eval_time],dtype='f')

		debug_output = data_from_simulator[1]

		if self.debug:
		 	print debug_output

		data_from_simulator = data_from_simulator[0]
		data_from_simulator = data_from_simulator.split()

		if ( data_from_simulator == [] ):
			return

		index = 0
		while ( data_from_simulator[index] != 'Done' ):
			sensor_id = int( data_from_simulator[index] )
			index = index + 1

			num_sensor_vals = int( data_from_simulator[index] ) 
			index = index + 1

			for t in range(0,self.eval_time): #time step
				for s in range(0,num_sensor_vals): #svi
					sensor_value = float( data_from_simulator[index] )
					self.data[sensor_id,s,t] = sensor_value
					index = index + 1

	def _send(self,command_string,*args):
		"""Send a command to the simulator"""

		string_to_send = command_string #first argument should always be a string
		for arg in args:
			string_to_send += ' ' + str(arg)
		string_to_send += '\n'

		if self.debug:
			print string_to_send
		self.strings_to_send.append(string_to_send)
