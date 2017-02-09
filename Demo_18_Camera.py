from pyrosim import PYROSIM
#ccappelle
#Same as Demo 17 except now you can move the initial camera

#xyz - x,y,z coordinate position of the camera
#hpr - head,pitch, and roll of the camera
xyz = (0.,-2., 2.0)
hpr = (90., -25., 0.0)

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

#set xyz,hpr in sim initialization
#defaults to start position being used before xyz=(0.8317,-0.9817,0.8),hpr= (121.0,-27.500,0.0)
#hints: always set r to 0. it is very disorienting if not 0,
#		h = 0 => facing along x-axis
#		h = 90=> facing along y-axis
sim = PYROSIM(playPaused = False , evalTime = 100, xyz=xyz,hpr=hpr)

sim.Send_Cylinder(objectID = 0 , x=0, y=0, z=ARM_LENGTH/2.0 + 2*ARM_RADIUS, r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Cylinder(objectID = 1 , x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + 2*ARM_RADIUS , r1=0, r2=1, r3=0, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Joint(jointID = 0, firstObjectID=0, secondObjectID=1, x=0, y=0, z=ARM_LENGTH + 2*ARM_RADIUS , n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

sim.Send_Joint(jointID = 1 , firstObjectID=0, secondObjectID=-1, x=0, y=0, z=ARM_LENGTH/2.0 + 2*ARM_RADIUS)

sim.Start()

