# demos/docs/composite_body.py
import pyrosim
import math

sim = pyrosim.Simulator(play_paused=True, eval_steps=-1)

# creates the composite entity
# use composite_id to reference this body later
composite_id = sim.send_composite_body()

# create a hollow box with cylinders for edges
scale = 0.25
height = 1.0
# create corner posts
for x in [-scale, scale]:
    for y in [-scale, scale]:
        sim.add_cylinder_to_composite(composite_id,
                                      position=[x, y,
                                                scale + scale / 5.0 + height],
                                      length=scale * 2.0,
                                      radius=scale / 5.0)

# create left and right sides
for x in [-scale, scale]:
    for z in [scale / 5.0, 2 * scale + scale / 5.0]:
        sim.add_cylinder_to_composite(composite_id,
                                      position=[x, 0, z + height],
                                      orientation=[0, 1, 0],
                                      length=scale * 2.0,
                                      radius=scale / 5.0)

# create front and back sides
for y in [-scale, scale]:
    for z in [scale / 5.0, 2 * scale + scale / 5.0]:
        sim.add_cylinder_to_composite(composite_id,
                                      position=[0, y, z + height],
                                      orientation=[1, 0, 0],
                                      length=scale * 2.0,
                                      radius=scale / 5.0)

sim.start()
sim.wait_to_finish()