from pyrosim import PYROSIM
import math

def myFunc(x):
	return math.sin(x*.3333)

def ONE(x):
	return 1

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = PYROSIM(playPaused = True , evalTime = 1000, dt=.05, debug=False, hpr=[121,5,0])

sim.Send_Cylinder(objectID = 0 , x=0, y=0, z=3.0*ARM_LENGTH, r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Cylinder(objectID = 1 , x=0, y=0 , z=2.0*ARM_LENGTH , r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Box(objectID = 2, x=0, y=0, z=1.5*ARM_LENGTH, length=ARM_RADIUS*10, width=ARM_RADIUS*7.0, height=ARM_RADIUS*7.0)

sim.Send_Joint(jointID = 0, firstObjectID=-1, secondObjectID=0, x=0, y=0, z=3.5*ARM_LENGTH, 
									n1=1, n2=0, n3=0, lo=-3.14159/2.0, hi=+3.14159/2.0, torque=10., speed=10., positionControl=True)

sim.Send_Joint(jointID = 1, firstObjectID=0, secondObjectID=1, x=0, y=0, z=2.5*ARM_LENGTH, 
									n1=1, n2=0, n3=0, lo=0, hi=0, torque=.1, speed=10.0)

sim.Send_Joint(jointID = 2, firstObjectID=1, secondObjectID=2, x=0, y=0, z=1.5*ARM_LENGTH)

sim.Send_Function_Neuron(neuronID=0, function=ONE)

sim.Send_Motor_Neuron(neuronID=1, jointID=0)
#sim.Send_Motor_Neuron(neuronID=1, jointID=1 )
sim.Send_Synapse(sourceNeuronID = 0 , targetNeuronID = 1 , weight = 1.0 )
#sim.Send_Synapse(sourceNeuronID = 0, targetNeuronID= 2, weight=1.0)

sim.Start()

#print sim.pipe.communicate()[0]
# sim.Collect_Sensor_Data()

