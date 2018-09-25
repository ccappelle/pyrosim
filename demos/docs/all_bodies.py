# demos/docs/all_bodies.py
import pyrosim

sim = pyrosim.Simulator(play_paused=True, eval_steps=-1)

sim.send_box(position=[-1, 0, 1])
sim.send_cylinder(position=[0, 0, 1])
sim.send_sphere(position=[1, 0, 1])

sim.start()
sim.wait_to_finish()