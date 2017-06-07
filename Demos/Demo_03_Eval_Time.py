import pyrosim

sim = pyrosim.Simulator(play_paused = True , eval_time = 1000)

sim.Send_Cylinder()

sim.Start()
