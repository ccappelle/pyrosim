import sys
sys.path.insert(0, '../../')
import pyrosim

# collision groups give you control over
# which bodies in the environment collide.
# You can specify them 2 ways:
# 1. Setting the collision group name in the
#    send method for each body
# 2. Setting the current collision group name
#
# You then use the assign_collision method
# to specify which groups can collide

sim = pyrosim.Simulator(eval_steps=500)

# ex of 1
sim.send_cylinder(position=(0, 0, 1),
                  collision_group='Cylinder')

# ex of 2
# send boxes with collision_group 'Box'
sim.set_current_collision_group('Box')
for i in range(5):
    sim.send_box(position=(0, 0, i + 1))

# send spheres with collision_group 'Sphere'
sim.set_current_collision_group('Sphere')
for i in range(5):
    sim.send_sphere(position=(0, 0, i + 6),
                    radius=0.1)

# only collide spheres with the cylinder
sim.assign_collision('Cylinder', 'Sphere')

sim.start()
sim.wait_to_finish()