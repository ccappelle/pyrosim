import pyrosim

sim = pyrosim.Simulator(play_paused=True, eval_time=1000)

sim.send_cylinder()

sim.start()
