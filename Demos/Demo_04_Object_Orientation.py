import pyrosim

sim = pyrosim.Simulator(play_paused = True , eval_time = 1000)

sim.Send_Cylinder(r1=0, r2=1, r3=0)

sim.Start()
