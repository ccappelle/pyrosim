import pyrosim

sims = {}

for s in range(0, 10):
    sims[s] = pyrosim.Simulator(play_paused=False, eval_time=1000)
    sims[s].send_cylinder(x=0, y=0, z=0.1, r1=0, r2=1,
                          r3=0, length=1.0, radius=0.1)
    sims[s].start()
