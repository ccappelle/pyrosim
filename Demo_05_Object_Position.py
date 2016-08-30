from pyrosim import PYROSIM

sim = PYROSIM(playPaused = True , evalTime = 1000)

sim.Send_Cylinder(x=0, y=0, z=0.5, r1=0, r2=1, r3=0)

sim.Start()
