import pyrosim

sim = pyrosim.Simulator(play_paused=True)

sim.Send_Cylinder()

sim.Start()
