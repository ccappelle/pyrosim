from pyrosim import PYROSIM

sim = PYROSIM(playPaused = True , evalTime = 1000)

sim.Send_Cylinder(r1=0, r2=1, r3=0)

sim.Start()
