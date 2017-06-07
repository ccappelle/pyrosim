import pyrosim

sim = pyrosim.Simulator(play_paused = True , eval_time = 1000)

x = 0
y = 0
z = 0.1

for i in range(0,10):
	sim.send_cylinder(x=x, y=y, z=z, r1=0, r2=1, r3=0, length=1.0, radius=0.1)
	z = z + 2.0 * 0.1

sim.start()
