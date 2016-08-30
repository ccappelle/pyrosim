from pyrosim import PYROSIM

sim = PYROSIM(playPaused = True , evalTime = 1000)

x = 0

y = 0

z = 0.1

for i in range(0,10):

	sim.Send_Cylinder(x=x, y=y, z=z, r1=0, r2=1, r3=0, length=1.0, radius=0.1)

	z = z + 2.0 * 0.1

sim.Start()
