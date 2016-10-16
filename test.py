from pyrosim import PYROSIM

sim = PYROSIM(playBlind=False,playPaused=False,evalTime=1000)

x = 0
y = 0
z = 0.05

for level in range(0,20):

	sim.Send_Box(ID=level, x=x, y=y, z=z, length=0.1, width=0.1, height=0.1, r=1, g=1, b=1)

	z = z + 0.1

x = 0.1
z = 0.05

for level in range(20,40):

        sim.Send_Box(ID=level, x=x, y=y, z=z, length=0.1, width=0.1, height=0.1, r=1, g=1, b=1)

        z = z + 0.1

sim.Start()
