from pyrosim import PYROSIM

sim = PYROSIM(playPaused = True , evalTime = 1000)

sim.Send_Cylinder()

sim.Start()
