# demos/docs/all_bodies_parameter_options.py
import pyrosim

sim = pyrosim.Simulator(play_paused=True, eval_steps=-1)

for i in range(3):
    y = i - 1
    color = [i / 2, i / 2, i / 2] # set the color to be greyscale
    size = 1 + i

    sim.send_box(position=[-1, y, 1],
                 sides=[size / 6, size / 6, size / 6],
                 color=color)

    sim.send_cylinder(position=[0, y, 1],
                      length=size / 6,
                      radius=size / 20,
                      color=color)

    sim.send_sphere(position=[1, y, 1],
                    radius=size / 6,
                    color=color)

sim.start()
sim.wait_to_finish()