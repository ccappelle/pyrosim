from pyrosim import PYROSIM

# Constants

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = PYROSIM(playPaused = True , evalTime = 1000)

# Upper arm

sim.Send_Cylinder(ID = 0 , x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS, r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

# Lower arm

sim.Send_Cylinder(ID = 1 , x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS, r1=0, r2=1, r3=0, length=ARM_LENGTH, radius=ARM_RADIUS)

# Joint connecting them

sim.Send_Joint(ID = 0, firstObjectID=0, secondObjectID=1, x=0, y=0, z=ARM_LENGTH + ARM_RADIUS, n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

# Put touch sensors in arm segments.

sim.Send_Touch_Sensor(ID = 0 , objectIndex = 0)

sim.Send_Touch_Sensor(ID = 1 , objectIndex = 1)

# Add sensor neurons and connect them to the sensors.

sim.Send_Sensor_Neuron(ID=0, sensorID=0, layer=0 )

sim.Send_Sensor_Neuron(ID=1, sensorID=1, layer=0 )

# Add a motor neuron and connect it to the joint.

sim.Send_Motor_Neuron(ID = 2 , jointID = 0 , layer = 1 )

sim.Start()
