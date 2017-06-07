import pyrosim

sim = pyrosim.Simulator(play_paused=True)

sim.send_cylinder()

sim.start()
