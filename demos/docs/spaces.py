# demos/docs/spaces.py
import pyrosim

# press 's' to see spaces
sim = pyrosim.Simulator(eval_steps=-1)

# setting space by parameter
for i in range(3):
    sim.send_box(position=[-1, i-1, 1],
                 sides=[.25, .25, (i + 1) / 2],
                 space='space1')

# setting by command
sim.set_current_space('space2')
for i in range(3):
    sim.send_cylinder(position=[1, i-1, 1],
                      length=(i + 1) / 2)

sim.start()
sim.wait_to_finish()