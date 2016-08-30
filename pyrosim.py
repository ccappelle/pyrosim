import math
import sys

import constants

from subprocess import Popen, PIPE

class PYROSIM:

	def __init__(self,playBlind=False,playPaused=False,evalTime=constants.evaluationTime):

		self.numJoints = 0

		self.numSensors = 0

		commandsToSend = ['./simulator']

		if ( playBlind == True ):

			commandsToSend.append('-blind')
		else:
			commandsToSend.append('-notex')

		if ( playPaused == True ):

			commandsToSend.append('-pause')

		self.simulator = Popen(commandsToSend, stdout=PIPE, stdin=PIPE, stderr=PIPE)

                # self.simulator = Popen(commandsToSend, stdout=PIPE, stdin=PIPE)

                self.Send('EvaluationTime '+str(evalTime)+'\n')

        def Get_Sensor_Data(self,ID,s,t):

                return self.dataFromPython[ID,s,t]

	def Send_Bias_Neuron(self, ID = 0 , layer = 0 ):

                outputString = 'BiasNeuron'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(layer)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Box(self, ID=0, x=0, y=0, z=0, length=0.1, width=0.1, height=0.1, r=1, g=1, b=1):

                outputString = 'Box'

		outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(x)
                outputString = outputString + ' ' + str(y)
                outputString = outputString + ' ' + str(z)

                outputString = outputString + ' ' + str(length)
                outputString = outputString + ' ' + str(width)
                outputString = outputString + ' ' + str(height)

                outputString = outputString + ' ' + str(r)
                outputString = outputString + ' ' + str(g)
                outputString = outputString + ' ' + str(b)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Cylinder(self, ID=0, x=0, y=0, z=0, r1=0, r2=0, r3=1, length=1.0, radius=0.1, r=1, g=1, b=1):

                outputString = 'Cylinder'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(x)
                outputString = outputString + ' ' + str(y)
                outputString = outputString + ' ' + str(z)

                outputString = outputString + ' ' + str(r1)
                outputString = outputString + ' ' + str(r2)
                outputString = outputString + ' ' + str(r3)

                outputString = outputString + ' ' + str(length)
                outputString = outputString + ' ' + str(radius)

                outputString = outputString + ' ' + str(r)
                outputString = outputString + ' ' + str(g)
                outputString = outputString + ' ' + str(b)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Hidden_Neuron(self, ID = 0 , layer = 0 , tau = 1.0 ):

		outputString = 'HiddenNeuron'

		outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(layer)

                outputString = outputString + ' ' + str(tau)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Joint(self, ID=0, firstObjectID=0, secondObjectID=1, x=0, y=0, z=0, n1=0, n2=0, n3=1, lo=-math.pi/4.0, hi=+math.pi/4.0 , speed=1.0):

                outputString = 'Joint'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(firstObjectID)
                outputString = outputString + ' ' + str(secondObjectID)

                outputString = outputString + ' ' + str(x)
                outputString = outputString + ' ' + str(y)
                outputString = outputString + ' ' + str(z)

                outputString = outputString + ' ' + str(n1)
                outputString = outputString + ' ' + str(n2)
                outputString = outputString + ' ' + str(n3)

                outputString = outputString + ' ' + str(lo)
                outputString = outputString + ' ' + str(hi)

                outputString = outputString + ' ' + str(speed)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Light_Sensor(self, ID, objectIndex = 0 ):

                outputString = 'LightSensor'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(objectIndex)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Light_Source(self, objectIndex = 0 ):

                outputString = 'LightSource'

                outputString = outputString + ' ' + str(objectIndex)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Motor_Neuron(self , ID = 0 , jointID = 0 , layer = 0 , tau = 1.0 ):

                outputString = 'MotorNeuron'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(jointID)

                outputString = outputString + ' ' + str(layer)

                outputString = outputString + ' ' + str(tau)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Position_Sensor(self, ID=0, objectIndex = 0):

		outputString = 'PositionSensor'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(objectIndex)

		outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Proprioceptive_Sensor(self, ID, jointIndex = 0):

                outputString = 'ProprioceptiveSensor'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(jointIndex)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Sensor_Neuron(self, ID=0, sensorID=0, sensorValueIndex=0, layer=0, tau=1.0 ):

		outputString = 'SensorNeuron'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(sensorID)

                outputString = outputString + ' ' + str(sensorValueIndex)

                outputString = outputString + ' ' + str(layer)

                outputString = outputString + ' ' + str(tau)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Ray_Sensor(self, ID=0, objectIndex=0, x=0,y=0,z=0, r1=0,r2=0,r3=1):

                outputString = 'RaySensor'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(objectIndex)

                outputString = outputString + ' ' + str(x)
                outputString = outputString + ' ' + str(y)
                outputString = outputString + ' ' + str(z)

                outputString = outputString + ' ' + str(r1)
                outputString = outputString + ' ' + str(r2)
                outputString = outputString + ' ' + str(r3)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Send_Synapse(self, sourceNeuronIndex = 0 , targetNeuronIndex = 0 , weight = 0.0 ):

	        outputString = 'Synapse'

                outputString = outputString + ' ' + str(sourceNeuronIndex)

                outputString = outputString + ' ' + str(targetNeuronIndex)

                outputString = outputString + ' ' + str(weight)

                outputString = outputString + '\n'

                self.Send(outputString)
	
        def Send_Touch_Sensor(self, ID, objectIndex):

                outputString = 'TouchSensor'

                outputString = outputString + ' ' + str(ID)

                outputString = outputString + ' ' + str(objectIndex)

                outputString = outputString + '\n'

                self.Send(outputString)

	def Send_Vestibular_Sensor(self, ID, objectIndex = 0):

                outputString = 'VestibularSensor'

                outputString = outputString + ' ' + str(ID)

                self.numSensors = self.numSensors + 1

                outputString = outputString + ' ' + str(objectIndex)

                outputString = outputString + '\n'

                self.Send(outputString)

        def Start(self):

                self.Send('Done\n')

        def Wait_To_Finish(self):

		dataFromSimulator = self.simulator.communicate()

                self.Collect_Sensor_Data(dataFromSimulator)

# --------------------- Private methods -----------------------------

	def Collect_Sensor_Data(self,dataFromSimulator):

		self.dataFromPython = {}

                dataFromSimulator = dataFromSimulator[0]

                dataFromSimulator = dataFromSimulator.split()

		index = 0

		while ( dataFromSimulator[index] != 'Done' ):

			ID = int( dataFromSimulator[index] )

			index = index + 1

			numSensorValues = int( dataFromSimulator[index] ) 

			index = index + 1

			for t in range(0,constants.evaluationTime):

                        	for s in range(0,numSensorValues):

					sensorValue = float( dataFromSimulator[index] )

					self.dataFromPython[ID,s,t] = sensorValue

					index = index + 1

	def Send(self,stringToSend):

                self.simulator.stdin.write( stringToSend )
