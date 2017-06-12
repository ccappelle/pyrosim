import pyrosim

sim = pyrosim.Simulator(play_paused=True, eval_time=1000)

sim.send_cylinder(x=0, y=0, z=0.5, r1=0, r2=1, r3=0)

sim.start()
