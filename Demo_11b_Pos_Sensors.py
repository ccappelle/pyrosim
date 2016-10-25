from pyrosim import PYROSIM

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = PYROSIM(playPaused = False , evalTime = 100)

sim.Send_Cylinder(ID = 0 , x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS, r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Cylinder(ID = 1 , x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS, r1=0, r2=1, r3=0, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Joint(ID = 0, firstObjectID=0, secondObjectID=1, x=0, y=0, z=ARM_LENGTH + ARM_RADIUS, n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

sim.Send_Touch_Sensor(ID = 0 , objectIndex = 0)

sim.Send_Touch_Sensor(ID = 1 , objectIndex = 1)

sim.Send_Position_Sensor(ID = 2 , objectIndex = 0)

sim.Start()

sim.Wait_To_Finish()
