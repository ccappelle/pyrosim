from pyrosim import PYROSIM

WHEEL_RADIUS = 0.1

sim = PYROSIM(playPaused = False , evalTime = 1000)

sim.Send_Cylinder(objectID = 0 , x=-2*WHEEL_RADIUS, y=-2*WHEEL_RADIUS, z=WHEEL_RADIUS, length=0, radius=WHEEL_RADIUS)
sim.Send_Cylinder(objectID = 1 , x=-2*WHEEL_RADIUS, y=+2*WHEEL_RADIUS, z=WHEEL_RADIUS, length=0, radius=WHEEL_RADIUS)

sim.Send_Cylinder(objectID = 2 , x=+2*WHEEL_RADIUS, y=-2*WHEEL_RADIUS, z=WHEEL_RADIUS, length=0, radius=WHEEL_RADIUS)
sim.Send_Cylinder(objectID = 3 , x=+2*WHEEL_RADIUS, y=+2*WHEEL_RADIUS, z=WHEEL_RADIUS, length=0, radius=WHEEL_RADIUS)

sim.Send_Box(     objectID = 4,  x=0,               y=0,               z=WHEEL_RADIUS, length=4*WHEEL_RADIUS, width=4*WHEEL_RADIUS, height=WHEEL_RADIUS)

sim.Send_Joint(   jointID  = 0, firstObjectID=0, secondObjectID=4, x=-2*WHEEL_RADIUS, y=-2*WHEEL_RADIUS, z=WHEEL_RADIUS, n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

#sim.Send_Touch_Sensor(sensorID = 0 , objectID = 0)
#sim.Send_Touch_Sensor(sensorID = 1 , objectID = 1)
#sim.Send_Sensor_Neuron(neuronID=0, sensorID=0 )
#sim.Send_Sensor_Neuron(neuronID=1, sensorID=1 )
#sim.Send_Motor_Neuron(neuronID=2 , jointID=0 )
#sim.Send_Synapse(sourceNeuronID = 0 , targetNeuronID = 2 , weight = 1.0 )
#sim.Send_Synapse(sourceNeuronID = 1 , targetNeuronID = 2 , weight = -1.0 )

sim.Start()
#sim.Collect_Sensor_Data()
