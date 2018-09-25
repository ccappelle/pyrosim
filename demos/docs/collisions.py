# demos/docs/collisions.py
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1)

# no collision -- default collision group
sim.send_cylinder(position=[-1, 0, 1])
sim.send_cylinder(position=[-1, 0, 2])

# setting collision group through parameters
sim.send_cylinder(position=[0, 0, 1],
                  collision_group='top')
sim.send_cylinder(position=[0, 0, 2],
                  collision_group='bottom')

# setting collision group through single statement
sim.set_current_collision_group('top')
sim.send_cylinder(position=[1, 0, 1])
sim.set_current_collision_group('bottom')
sim.send_cylinder(position=[1, 0, 2])

# assign the collision to occur
sim.assign_collision('top', 'bottom')

sim.start()
sim.wait_to_finish()